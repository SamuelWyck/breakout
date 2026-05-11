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
    ImageStatTracker m_playerHealthDisplay{
        healthDisplayX, healthDisplayY, healthDisplayGap,  Framework::images.ballImg, nullptr
    };


public:
    Hud(Player* playerPtr) {
        auto healthCb {[playerPtr]() -> int {
            return playerPtr->getHealth();
        }};
        m_playerHealthDisplay.setGetterCb(healthCb);
    };


    void update(SDL_Renderer* renderer) {
        m_playerHealthDisplay.update(renderer);
    };
};



#endif