#ifndef HUD_H
#define HUD_H


#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include "../entities/player.h"
#include "./imageStatTracker.h"
#include "./liveTextDisplay.h"
#include "../framework/framework.h"


class Hud {
    static constexpr float healthDisplayX {25};
    static constexpr float healthDisplayY {25};
    static constexpr float healthDisplayGap {10};
    static constexpr int (*healthGetterCb)() {nullptr};
    ImageStatTracker m_playerHealthDisplay{
        healthDisplayX, healthDisplayY, healthDisplayGap,  Framework::images.ballImg, healthGetterCb
    };

    LiveTextDisplay m_scoreDisplay{0, 0, &Framework::fonts.scoreFont, SDL_Color{255, 255, 255, 255}, nullptr};


public:
    Hud(Player* playerPtr, int canvasWidth, int canvasHeight);

    void update(SDL_Renderer* renderer);
};



#endif