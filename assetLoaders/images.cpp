#include <filesystem>
#include <string>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>
#include "./images.h"



Images::Images(SDL_Renderer* renderer) {
    namespace fs = std::filesystem;
    std::string blockImgPath {"./assets/images/blockImages"};
    std::string entityImgPath {"./assets/images/entityImages"};
    std::string otherImgPath {"./assets/images/otherImages"};

    // block images

    basicBlockImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{blockImgPath + "/basicBlock.png"}).string().data());


    // non block entites

    playerImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{entityImgPath + "/player.png"}).string().data());


    // other images

    backgroundImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{otherImgPath + "/backgroundImg.png"}).string().data());
};



Images::~Images() {
    SDL_DestroyTexture(basicBlockImg);
};