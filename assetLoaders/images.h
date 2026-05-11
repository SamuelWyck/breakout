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

    

    Images(SDL_Renderer* renderer);

    Images(const Images&) = delete;
    Images(Images&&) = delete;
    Images& operator=(const Images&) = delete;
    Images& operator=(Images&&) = delete;

    ~Images();
};



#endif