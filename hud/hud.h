#ifndef HUD_H
#define HUD_H


#include <SDL3/SDL_render.h>
#include "../entities/player.h"
#include "./imageStatTracker.h"
#include "../framework/framework.h"


class Hud {
    static constexpr float healthDisplayX {25};
    static constexpr float healthDisplayY {25};
    static constexpr float healthDisplayGap {10};
    static constexpr int (*healthGetterCb)() {nullptr};
    ImageStatTracker m_playerHealthDisplay{
        healthDisplayX, healthDisplayY, healthDisplayGap,  Framework::images.ballImg, healthGetterCb
    };


public:
    Hud(Player* playerPtr);

    void update(SDL_Renderer* renderer);
};



#endif