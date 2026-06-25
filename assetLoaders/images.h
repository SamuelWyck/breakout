#ifndef IMAGES_H
#define IMAGES_H


#include <vector>
#include <SDL3/SDL_render.h>
#include <SDl3/SDL_surface.h>


class Images {
    std::vector<SDL_Texture*> m_images{};


public:
    SDL_Texture* basicBlockImg {nullptr};
    SDL_Texture* hardBlockImg {nullptr};
    SDL_Texture* hardBlockHitImg {nullptr};
    SDL_Texture* wallBlockImg {nullptr};
    SDL_Texture* ballBlockImg {nullptr};
    SDL_Surface* ghostBlockMask {nullptr};

    SDL_Texture* playerImg {nullptr};
    SDL_Texture* ballImg {nullptr};
    
    SDL_Texture* backgroundImg {nullptr};
    SDL_Texture* mouseImg {nullptr};

    SDL_Texture* mainMenuImg {nullptr};
    SDL_Texture* playBtnImg {nullptr};
    SDL_Texture* playBtnHvrImg {nullptr};
    SDL_Texture* settingsBtnImg {nullptr};
    SDL_Texture* settingsBtnHvrImg {nullptr};
    SDL_Texture* exitBtnImg {nullptr};
    SDL_Texture* exitBtnHvrImg {nullptr};
    SDL_Texture* settingsMenuImg {nullptr};
    SDL_Texture* audioBtnImg {nullptr};
    SDL_Texture* audioBtnHvrImg {nullptr};
    SDL_Texture* mouseBtnImg {nullptr};
    SDL_Texture* mouseBtnHvrImg {nullptr};
    SDL_Texture* controlsBtnImg {nullptr};
    SDL_Texture* controlsBtnHvrImg {nullptr};
    SDL_Texture* backBtnImg {nullptr};
    SDL_Texture* backBtnHvrImg {nullptr};
    SDL_Texture* slideBarImg {nullptr};
    SDL_Texture* slideImg {nullptr};
    SDL_Texture* audioMenuImg {nullptr};

    

    Images(SDL_Renderer* renderer);

    Images(const Images&) = delete;
    Images(Images&&) = delete;
    Images& operator=(const Images&) = delete;
    Images& operator=(Images&&) = delete;

    ~Images();
};



#endif