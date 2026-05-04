#ifndef FRAMEWORK_H
#define FRAMEWORK_H


#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "./display.h"


namespace Framework {
    // pass screen size, canvas size, window flags, and renderer flags 

    inline static Display display{
        0, 0, // screen size
        1536, 864, // canvas size
        SDL_WINDOW_FULLSCREEN | SDL_WINDOW_MOUSE_GRABBED | SDL_WINDOW_MOUSE_RELATIVE_MODE, // window flags 
        SDL_LOGICAL_PRESENTATION_STRETCH // renderer presentation flag
    };
};



#endif