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
    std::string menuImgPath {"assets/images/menuImages"};

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

    backgroundImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + otherImgPath + "/background.png"}.data());
    m_images.push_back(backgroundImg);
    mouseImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + otherImgPath + "/mouse.png"}.data());


    // menu images

    mainMenuImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/mainMenu.png"}.data());
    m_images.push_back(mainMenuImg);
    playBtnImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/playBtn.png"}.data());
    m_images.push_back(playBtnImg);
    playBtnHvrImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/playBtnHvr.png"}.data());
    m_images.push_back(playBtnHvrImg);
    settingsBtnImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/settingsBtn.png"}.data());
    m_images.push_back(settingsBtnImg);
    settingsBtnHvrImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/settingsBtnHvr.png"}.data());
    m_images.push_back(settingsBtnHvrImg);
    exitBtnImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/exitBtn.png"}.data());
    m_images.push_back(exitBtnImg);
    exitBtnHvrImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/exitBtnHvr.png"}.data());
    m_images.push_back(exitBtnHvrImg);
    settingsMenuImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/settingsMenu.png"}.data());
    m_images.push_back(settingsMenuImg);
    audioBtnImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/audioBtn.png"}.data());
    m_images.push_back(audioBtnImg);
    audioBtnHvrImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/audioBtnHvr.png"}.data());
    m_images.push_back(audioBtnHvrImg);
    mouseBtnImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/mouseBtn.png"}.data());
    m_images.push_back(mouseBtnImg);
    mouseBtnHvrImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/mouseBtnHvr.png"}.data());
    m_images.push_back(mouseBtnHvrImg);
    controlsBtnImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/controlsBtn.png"}.data());
    m_images.push_back(controlsBtnImg);
    controlsBtnHvrImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/controlsBtnHvr.png"}.data());
    m_images.push_back(controlsBtnHvrImg);
    backBtnImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/backBtn.png"}.data());
    m_images.push_back(backBtnImg);
    backBtnHvrImg = IMG_LoadTexture(renderer, std::string{SDL_GetBasePath() + menuImgPath + "/backBtnHvr.png"}.data());
    m_images.push_back(backBtnHvrImg);
};



Images::~Images() {
    for (SDL_Texture* img : m_images) {
        SDL_DestroyTexture(img);
    }

    SDL_DestroySurface(ghostBlockMask);
};