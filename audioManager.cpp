#include <filesystem>
#include <SDL3/SDL.h>
#include "./audioManager.h"
#include "./sdlUtils/display.h"




int main() {
    namespace fs = std::filesystem;
    Display display {"Test", 0, 0, 1536, 864, SDL_WINDOW_FULLSCREEN};
    AudioManager audioManager{{}, {}, {}, fs::absolute(fs::path{"./menu_music.ogg"}).string()};

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
    }
    return 0;
};