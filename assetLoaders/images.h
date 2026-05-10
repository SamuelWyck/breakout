#ifndef IMAGES_H
#define IMAGES_H


#include <SDL3/SDL_render.h>


class Images {
public:
    SDL_Texture* basicBlockImg {nullptr};
    SDL_Texture* playerImg {nullptr};
    SDL_Texture* backgroundImg {nullptr};
    SDL_Texture* ballImg {nullptr};

    

    Images(SDL_Renderer* renderer);

    Images(const Images&) = delete;
    Images(Images&&) = delete;
    Images& operator=(const Images&) = delete;
    Images& operator=(Images&&) = delete;

    ~Images();
};



#endif