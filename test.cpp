#include "./sdlUtils/userInterface/elements/button.h"
#include "./sdlUtils/userInterface/elements/slider.h"
#include "./sdlUtils/userInterface/elements/textDisplay.h"
#include "./sdlUtils/userInterface/elements/liveTextDisplay.h"

#include <SDL3/SDL.h>
#include <string>
#include <format>
#include <SDL3_image/SDL_image.h>
#include "./framework/framework.h"
#include "./sdlUtils/userInterface/mouse.h"
#include <iostream>
#include "./sdlUtils/color.h"
#include "./sdlUtils/userInterface/buttonMenu.h"



int main() {

    SDL_Texture* bgImg {IMG_LoadTexture(Framework::display.renderer(), std::string{SDL_GetBasePath() + std::string{"audio_menu_bg.png"}}.data())};
    SDL_Texture* img {IMG_LoadTexture(Framework::display.renderer(), std::string{SDL_GetBasePath() + std::string{"play_btn.png"}}.data())};
    SDL_Texture* imgHover {IMG_LoadTexture(Framework::display.renderer(), std::string{SDL_GetBasePath() + std::string{"play_btn_hover.png"}}.data())};
    SDL_Texture* imgBar {IMG_LoadTexture(Framework::display.renderer(), std::string{SDL_GetBasePath() + std::string{"slider_bar.png"}}.data())};
    SDL_SetTextureScaleMode(imgBar, SDL_SCALEMODE_PIXELART);
    SDL_Texture* imgSlide {IMG_LoadTexture(Framework::display.renderer(), std::string{SDL_GetBasePath() + std::string{"slider_slide.png"}}.data())};

    Mouse mouse{
        static_cast<float>(Framework::display.screenWidth()),
        static_cast<float>(Framework::display.screenHeight()),
        static_cast<float>(Framework::display.canvasWidth()),
        static_cast<float>(Framework::display.canvasHeight()),
        Framework::images.ballImg,
        Framework::images.ballImg->w / 2.0f,
        Framework::images.ballImg->h / 2.0f,
    };
    mouse.clampMouseToCanvas(true);
    SDL_SetWindowRelativeMouseMode(Framework::display.window(), true);

    Button button{400, 400, img, imgHover};

    std::function<void(float)> sliderCb{
        [](float val) -> void {
            std::cout << val << "\n";
        }
    };

    Slider slider{100, 100, sliderCb, imgBar, imgSlide};
    slider.setValue(1);


    TextDisplay hardText{800, 400, "testing", &Framework::fonts.scoreFont, Color{255, 0, 0, 255}};


    std::function<std::string()> liveTextCb{
        [&slider]() -> std::string {
            float val {slider.value()};
            return std::format("{:.2f}", val);
        }
    };

    LiveTextDisplay liveText{500, 100, &Framework::fonts.scoreFont, Color{0, 255, 0, 255}, liveTextCb};


    std::vector<Button*> btns{};
    btns.push_back(new Button{button});
    btns.push_back(new Button{button});

    std::vector<MenuCb> cbs{};
    cbs.push_back({
        [](SDL_Renderer*, SDL_Texture*) -> MenuReturn {
            return {{0, -1}};
        }
    });
    cbs.push_back({
        [](SDL_Renderer*, SDL_Texture*) -> MenuReturn {
            return {{1, -1}};
        }
    });

    ButtonMenu menu{btns, cbs, 400, 400, 200, &mouse};


    FRect rect{600, 400, 50, 50};
    bool toggle {false};

    bool running {true};
    bool prepMenuTexture {false};
    bool enterMenu {false};

    // Framework::display.targetTexture();
    // SDL_SetRenderDrawColor(Framework::display.renderer(), 255, 0, 0, 255);
    // SDL_RenderFillRect(Framework::display.renderer(), nullptr);
    // SDL_Texture* screenBg{Framework::display.getScreenTexture()};

    while (running) {
        bool mousePressed {false};
        bool mouseReleased {false};

        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.scancode == SDL_SCANCODE_BACKSPACE) {
                    running = false;
                } else if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    prepMenuTexture = true;
                }
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                mouse.update(event.motion.xrel, event.motion.yrel);
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mousePressed = true;
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseReleased = true;
                }
            }
        }


        if (enterMenu) {
            enterMenu = false;
            menu.run(Framework::display.renderer(), Framework::display.getScreenTexture());
        }
        if (prepMenuTexture) {
            Framework::display.targetScreenTexture();
        }


        SDL_FPoint mousePos {mouse.getPos()};


        SDL_SetRenderDrawColor(Framework::display.renderer(), 150, 150, 150, 255);
        SDL_RenderClear(Framework::display.renderer());

        button.update(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);

        if (button.clicked()) {
            toggle = !toggle;
        }

        int red {(toggle) ? 0 : 255};
        int green {(toggle) ? 255 : 0};

        SDL_SetRenderDrawColor(Framework::display.renderer(), red, green, 0, 255);
        SDL_RenderFillRect(Framework::display.renderer(), &rect.getSDLFRect());
        slider.update(Framework::display.renderer(), mousePos, mousePressed, mouseReleased);
        hardText.update(Framework::display.renderer());
        liveText.update(Framework::display.renderer());
        mouse.draw(Framework::display.renderer());

        if (prepMenuTexture) {
            Framework::display.targetWindow();
            prepMenuTexture = false;
            enterMenu = true;
        }

        SDL_RenderPresent(Framework::display.renderer());
    }

    SDL_DestroyTexture(img);
    SDL_DestroyTexture(imgHover);
    SDL_DestroyTexture(imgBar);
    SDL_DestroyTexture(imgSlide);
    SDL_DestroyTexture(bgImg);

    return 0;
};