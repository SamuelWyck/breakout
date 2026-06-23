#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_filesystem.h>
#include "../sdlUtils/display.h"
#include "../sdlUtils/audio/audioManager.h"
#include "../assetLoaders/images.h"
#include "../assetLoaders/fonts.h"
#include "../assetLoaders/sounds.h"



namespace Framework {
    // pass screen size, canvas size, window flags, and renderer flags 

    Display display{
        "Breakout",
        0, 0, // screen size
        1536, 864, // canvas size
        1, // vsync rate
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MOUSE_GRABBED | SDL_WINDOW_MOUSE_RELATIVE_MODE, // window flags 
        SDL_LOGICAL_PRESENTATION_STRETCH // renderer presentation flag
    };


    Images images{display.renderer()};

    Fonts fonts{};

    AudioManager audioManager{
        {"BALL"}, 
        {
            {"BALL_SOUND", SDL_GetBasePath() + std::string{"assets/sounds/ballBounce.wav"}}
        }
    };

    Sounds sounds{audioManager};
};