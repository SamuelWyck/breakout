#include <string>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "./images.h"



Images::Images(SDL_Renderer* renderer) {
    std::string blockImgPath {"assets/images/blockImages"};
    std::string entityImgPath {"assets/images/entityImages"};
    std::string otherImgPath {"assets/images/otherImages"};

    // block images

    basicBlockImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + blockImgPath + "/basicBlock.png"}.data());
    m_images.push_back(backgroundImg);
    hardBlockImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + blockImgPath + "/hardBlock.png"}.data());
    m_images.push_back(hardBlockImg);
    hardBlockHitImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + blockImgPath + "/hardBlockHit.png"}.data());
    m_images.push_back(hardBlockHitImg);
    wallBlockImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + blockImgPath + "/wallBlock.png"}.data());
    m_images.push_back(wallBlockImg);
    ballBlockImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + blockImgPath + "/ballBlock.png"}.data());
    m_images.push_back(ballBlockImg);

    ghostBlockMask = SDL_LoadSurface(std::string{SDL_GetBasePath() + blockImgPath + "/ghostBlockMask.png"}.data());


    // non block entites

    playerImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + entityImgPath + "/player.png"}.data());
    m_images.push_back(playerImg);
    ballImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + entityImgPath + "/ball.png"}.data());
    m_images.push_back(ballImg);


    // other images

    backgroundImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + otherImgPath + "/backgroundImg.png"}.data());
    m_images.push_back(backgroundImg);
};



Images::~Images() {
    for (SDL_Texture* img : m_images) {
        SDL_DestroyTexture(img);
    }

    SDL_DestroySurface(ghostBlockMask);
};