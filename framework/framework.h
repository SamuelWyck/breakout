#ifndef FRAMEWORK_H
#define FRAMEWORK_H


#include <SDL3/SDL_render.h>
#include "./display.h"


namespace Framework {
    inline static Display display{1536, 864, SDL_LOGICAL_PRESENTATION_STRETCH};
};



#endif