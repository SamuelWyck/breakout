#include <functional>
#include <vector>
#include <string>
#include <format>
#include "./ui.h"
#include "../framework/framework.h"
#include "../sdlUtils/userInterface/buttonMenu.h"
#include "../sdlUtils/userInterface/generalMenu.h"
#include "../sdlUtils/userInterface/elements/button.h"
#include "../sdlUtils/userInterface/elements/slider.h"
#include "../sdlUtils/userInterface/elements/textDisplay.h"
#include "../sdlUtils/userInterface/elements/elementGap.h"
#include "../sdlUtils/userInterface/elements/liveTextDisplay.h"
#include "../sdlUtils/userInterface/elements/iMenuElement.h"
#include "../sdlUtils/userInterface/menuTypes.h"
#include "../sdlUtils/userInterface/mouse.h"
#include "../sdlUtils/audio/audioManager.h"
#include "../framework/colors.h"



Ui::Ui(Mouse* mouse, AudioManager* audio) {
    audioMenu = createAudioMenu(mouse, audio);
    settingsMenu = createSettingsMenu(mouse);
    mainMenu = createMainMenu(mouse);    
};

Ui::~Ui() {
    delete mainMenu;
    delete settingsMenu;
    delete audioMenu;
};


ButtonMenu* Ui::createMainMenu(Mouse* mouse) {
    Images& images{Framework::images};

    std::vector<Button*> menuButtons{
        new Button{0, 0, images.playBtnImg, images.playBtnHvrImg},
        new Button{0, 0, images.settingsBtnImg, images.settingsBtnHvrImg},
        new Button{0, 0, images.exitBtnImg, images.exitBtnHvrImg}
    };

    std::vector<MenuCb> menuBtnCallbacks{
        [](SDL_Renderer*, int, SDL_Surface*) -> MenuReturn {
            return {{1, -1}};
        },
        [this](SDL_Renderer* renderer, int framerate, SDL_Surface* bgCanvas) -> MenuReturn {
            return this->settingsMenu->run(renderer, framerate, bgCanvas);
        },
        [](SDL_Renderer*, int, SDL_Surface*) -> MenuReturn {
            return {{2, -1}};
        }
    };

    constexpr float btnX {40};
    constexpr float btnY {500};
    constexpr float btnGap {20};

    ButtonMenu* menu{new ButtonMenu{menuButtons, menuBtnCallbacks, btnX, btnY, btnGap, mouse, images.mainMenuImg}};
    return menu;
};


ButtonMenu* Ui::createSettingsMenu(Mouse* mouse) {
    Images& images{Framework::images};

    std::vector<Button*> btns{
        new Button{0, 0, images.audioBtnImg, images.audioBtnHvrImg},
        new Button{0, 0, images.mouseBtnImg, images.mouseBtnHvrImg},
        new Button{0, 0, images.controlsBtnImg, images.controlsBtnHvrImg},
        new Button{0, 0, images.backBtnImg, images.backBtnHvrImg}
    };

    std::vector<MenuCb> callbacks{
        [this](SDL_Renderer* renderer, int framerate, SDL_Surface* bgCanvas) -> MenuReturn {
            return this->audioMenu->run(renderer, framerate, bgCanvas);
        },
        [](SDL_Renderer*, int, SDL_Surface*) -> MenuReturn {
            return {{0, -1}};
        },
        [](SDL_Renderer*, int, SDL_Surface*) -> MenuReturn {
            return {{0, -1}};
        },
        [](SDL_Renderer*, int, SDL_Surface*) -> MenuReturn {
            return {{1, -1}};
        }
    };

    constexpr float btnX {40};
    constexpr float btnY {425};
    constexpr float btnGap {20};
    ButtonMenu* menu{new ButtonMenu{btns, callbacks, btnX, btnY, btnGap, mouse, images.settingsMenuImg}};
    return menu;
};


GeneralMenu* Ui::createAudioMenu(Mouse* mouse, AudioManager* audio) {
    Images& images{Framework::images};

    Slider* musicSlider {new Slider{
        0, 0, 
        [audio](float value) -> void {
            audio->setMusicVolume(value);
        },
        images.slideBarImg, images.slideImg
    }};

    Slider* soundsSlider {new Slider{
        0, 0,
        [audio](float value) -> void {
            audio->setSoundVolume(value);
        },
        images.slideBarImg, images.slideImg
    }};


    auto initCb{
        [audio, soundsSlider, musicSlider]() -> void {
            soundsSlider->setValue(audio->getSoundVolume());
            musicSlider->setValue(audio->getMusicVolume());
        }
    };


    auto cleanupCb{
        [audio] -> void {
            audio->saveVolumeSettings();
        }
    };


    std::vector<IMenuElement*> elements{
        new TextDisplay{0, 0, "MUSIC", &Framework::fonts.menuFont, Colors::WHITE},
        new LiveTextDisplay{
            0, 0, &Framework::fonts.menuSmallFont, Colors::WHITE, 
            [musicSlider]() -> std::string {
                return std::format("{:.2f}", musicSlider->value());
            }
        },
        musicSlider,
        new ElementGap{60},
        new TextDisplay{0, 0, "SOUNDS", &Framework::fonts.menuFont, Colors::WHITE},
        new LiveTextDisplay{
            0, 0, &Framework::fonts.menuSmallFont, Colors::WHITE, 
            [soundsSlider]() -> std::string {
                return std::format("{:.2f}", soundsSlider->value());
            }
        },
        soundsSlider,
        new ElementGap{60},
        new Button{0, 0, images.backBtnImg, images.backBtnHvrImg}
    };


    constexpr float yStart {220};
    constexpr float elementGap {10};
    constexpr float bgImgCoord {0.0f};
    GeneralMenu* menu{new GeneralMenu{
        Framework::display.canvasWidth() / 2.0f,
        yStart, 
        elementGap, 
        elements,
        {
            [](SDL_Renderer*, int, SDL_Surface*) -> MenuReturn {
                return {{1, -1}};
            }
        },
        mouse,
        images.audioMenuImg,
        bgImgCoord,
        bgImgCoord,
        cleanupCb,
        initCb
    }};

    return menu;
};