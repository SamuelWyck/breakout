#ifndef FONT_LOADER_H
#define FONT_LOADER_H


#include <string>
#include <SDL3/SDL_filesystem.h>
#include "../sdlUtils/font.h"


class Fonts {
    static constexpr std::string fontsDirPath{"assets/fonts"};


public:
    Font scoreFont{SDL_GetBasePath() + fontsDirPath + "/forge.ttf", 40};
    Font menuFont{SDL_GetBasePath() + fontsDirPath + "/menu.ttf", 40};
    Font menuSmallFont{SDL_GetBasePath() + fontsDirPath + "/menu.ttf", 30};
};



#endif