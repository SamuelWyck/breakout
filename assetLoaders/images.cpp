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
    m_images.push_back(backgroundImg);


    // non block entites

    playerImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{entityImgPath + "/player.png"}).string().data());
    m_images.push_back(playerImg);
    ballImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{entityImgPath + "/ball.png"}).string().data());
    m_images.push_back(ballImg);


    // other images

    backgroundImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{otherImgPath + "/backgroundImg.png"}).string().data());
    m_images.push_back(backgroundImg);
};



Images::~Images() {
    for (SDL_Texture* img : m_images) {
        SDL_DestroyTexture(img);
    }
};