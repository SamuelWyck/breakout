#ifndef FRAMEWORK_H
#define FRAMEWORK_H


#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "../assetLoaders/images.h"
#include "../sdlUtils/display.h"


namespace Framework {
    extern Display display;
    extern Images images;
};



#endif