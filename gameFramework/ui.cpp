#include <functional>
#include <vector>
#include "./ui.h"
#include "../framework/framework.h"
#include "../sdlUtils/userInterface/buttonMenu.h"
#include "../sdlUtils/userInterface/elements/button.h"
#include "../sdlUtils/userInterface/menuTypes.h"
#include "../sdlUtils/userInterface/mouse.h"



Ui::Ui(Mouse* mouse) {
    mainMenu = createMainMenu(mouse);    
};

Ui::~Ui() {
    delete mainMenu;
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
        [](SDL_Renderer*, int, SDL_Surface*) -> MenuReturn {
            return {{0, -1}};
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