#include <filesystem>
#include <string>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>
#include "./images.h"



Images::Images(SDL_Renderer* renderer) {
    namespace fs = std::filesystem;
    std::string blockImgPath {"./assets/images/blockImages"};

    // block images

    basicBlockImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{blockImgPath + "/basicBlock.png"}).string().data());
};



Images::~Images() {
    SDL_DestroyTexture(basicBlockImg);
};