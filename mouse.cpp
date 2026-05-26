#include "./framework/framework.h"
#include <SDL3/SDL.h>
#include <iostream>
#include "./mouse.h"




int main() {

    bool running {true};
    Mouse mouse{
        Framework::images.ballImg, 
        static_cast<float>(Framework::display.screenWidth()), 
        static_cast<float>(Framework::display.screenHeight()),
        static_cast<float>(Framework::display.canvasWidth()),
        static_cast<float>(Framework::display.canvasHeight()),
        Framework::images.ballImg->w / 2.0f,
        Framework::images.ballImg->h / 2.0f
    };

    FRect screenRect {
        0, 0, 
        static_cast<float>(Framework::display.canvasWidth()),
        static_cast<float>(Framework::display.canvasHeight())
    };

    // SDL_SetWindowRelativeMouseMode(Framework::display.window(), true);
    while (running) {
        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;

            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.scancode == SDL_SCANCODE_BACKSPACE) {
                    running = false;
                } else if (event.key.scancode == SDL_SCANCODE_Q) {
                    mouse.setSensitivity(mouse.getSensitivity() - .1f);
                } else if (event.key.scancode == SDL_SCANCODE_E) {
                    mouse.setSensitivity(mouse.getSensitivity() + .1f);
                }
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                mouse.update(event.motion.xrel, event.motion.yrel, &screenRect);
            }
        }


        SDL_SetRenderDrawColor(Framework::display.renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Framework::display.renderer());

        mouse.draw(Framework::display.renderer());

        SDL_RenderPresent(Framework::display.renderer());
    }


    return 0;
};