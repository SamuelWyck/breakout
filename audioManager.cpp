#include <filesystem>
#include <SDL3/SDL.h>
#include "./audioManager.h"
// #include "./sdlUtils/display.h"
#include "./sdlUtils/fCircle.h"
#include "./framework/framework.h"




int main() {
    namespace fs = std::filesystem;
    // Display display {"Test", 0, 0, 1536, 864, SDL_WINDOW_FULLSCREEN};
    AudioManager audioManager{{}, {}, {}, fs::absolute(fs::path{"./menu_music.ogg"}).string()};

    FRect rect{500, 500, 200, 500};
    FCircle circle{0, 0, 14};
    FRect circleRect{0, 0, 30, 30};

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
            }
        }


        float mX;
        float mY;
        SDL_GetGlobalMouseState(&mX, &mY);
        circle.setCenter(mX, mY);
        circleRect.setCenter(circle.center());


        int red {255};
        int green {0};
        if (circle.hasRectIntersection(&rect)) {
            red = 0;
            green = 255;
        }


        SDL_SetRenderDrawColor(Framework::display.renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Framework::display.renderer());
        
        SDL_SetRenderDrawColor(Framework::display.renderer(), red, green, 0, 255);
        SDL_RenderFillRect(Framework::display.renderer(), &rect.getSDLFRect());
        
        SDL_RenderTexture(Framework::display.renderer(), Framework::images.ballImg, nullptr, &circleRect.getSDLFRect());

        SDL_RenderPresent(Framework::display.renderer());
    }
    return 0;
};