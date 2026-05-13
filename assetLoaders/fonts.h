#ifndef FONT_LOADER_H
#define FONT_LOADER_H


#include <string>
#include <filesystem>
#include "../sdlUtils/font.h"


class Fonts {
    using path = std::filesystem::path;

    static constexpr std::string fontsDirPath{"./assets/fonts"};


public:
    Font scoreFont{std::filesystem::absolute(path{fontsDirPath + "/forge.ttf"}).string(), 40};
};



#endif