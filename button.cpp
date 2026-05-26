#include "./button.h"

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
    };

    Button button{400, 400, img, imgHover};


    bool running {true};

    while (running) {
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
            }
        }


        SDL_FPoint mousePos {mouse.getPos()};


        SDL_SetRenderDrawColor(Framework::display.renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Framework::display.renderer());

        button.update(Framework::display.renderer(), mousePos, false, false);
        SDL_RenderPresent(Framework::display.renderer());
    }

    SDL_DestroyTexture(img);
    SDL_DestroyTexture(imgHover);

    return 0;
};