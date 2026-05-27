#include "./sdlUtils/userInterface/elements/button.h"

#include <SDL3/SDL.h>
#include <string>
#include <SDL3_image/SDL_image.h>
#include "./framework/framework.h"
#include "./sdlUtils/userInterface/mouse.h"



int main() {

    SDL_Texture* img {IMG_LoadTexture(Framework::display.renderer(), std::string{SDL_GetBasePath() + std::string{"play_btn.png"}}.data())};
    SDL_Texture* imgHover {IMG_LoadTexture(Framework::display.renderer(), std::string{SDL_GetBasePath() + std::string{"play_btn_hover.png"}}.data())};

    Mouse mouse{
        static_cast<float>(Framework::display.screenWidth()),
        static_cast<float>(Framework::display.screenHeight()),
        static_cast<float>(Framework::display.canvasWidth()),
        static_cast<float>(Framework::display.canvasHeight()),
        Framework::images.ballImg,
        Framework::images.ballImg->w / 2.0f,
        Framework::images.ballImg->h / 2.0f,
    };

    Button button{400, 400, img, imgHover};

    FRect rect{600, 400, 50, 50};
    bool toggle {false};

    bool running {true};
    // mouse.setPositionToCursor();
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
        mouse.draw(Framework::display.renderer());


        SDL_RenderPresent(Framework::display.renderer());
    }

    SDL_DestroyTexture(img);
    SDL_DestroyTexture(imgHover);

    return 0;
};