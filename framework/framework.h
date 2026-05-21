#ifndef FRAMEWORK_H
#define FRAMEWORK_H


#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "../assetLoaders/images.h"
#include "../assetLoaders/fonts.h"
#include "../sdlUtils/display.h"
#include "../sdlUtils/audioManager.h"
#include "../assetLoaders/sounds.h"


namespace Framework {
    extern Display display;
    extern Images images;
    extern Fonts fonts;
    extern AudioManager audioManager;
    extern Sounds sounds;
};



#endif