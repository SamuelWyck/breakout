#include <filesystem>
#include <string>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "./images.h"



Images::Images(SDL_Renderer* renderer) {
    namespace fs = std::filesystem;
    std::string blockImgPath {"./assets/images/blockImages"};
    std::string entityImgPath {"./assets/images/entityImages"};
    std::string otherImgPath {"./assets/images/otherImages"};

    // block images

    basicBlockImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{blockImgPath + "/basicBlock.png"}).string().data());
    m_images.push_back(backgroundImg);
    hardBlockImg = IMG_LoadTexture(renderer, fs::absolute(fs::path(blockImgPath + "/hardBlock.png")).string().data());
    m_images.push_back(hardBlockImg);
    hardBlockHitImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{blockImgPath + "/hardBlockHit.png"}).string().data());
    m_images.push_back(hardBlockHitImg);
    wallBlockImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{blockImgPath + "/wallBlock.png"}).string().data());
    m_images.push_back(wallBlockImg);
    ballBlockImg = IMG_LoadTexture(renderer, fs::absolute(fs::path{blockImgPath + "/ballBlock.png"}).string().data());
    m_images.push_back(ballBlockImg);

    ghostBlockMask = SDL_LoadSurface(fs::absolute(fs::path{blockImgPath + "/ghostBlockMask.png"}).string().data());


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

    SDL_DestroySurface(ghostBlockMask);
};