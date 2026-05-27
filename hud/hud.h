#ifndef HUD_H
#define HUD_H


#include <SDL3/SDL_render.h>
#include "../entities/player.h"
#include "./imageStatTracker.h"
#include "../sdlUtils/userInterface/elements/liveTextDisplay.h"
#include "../sdlUtils/userInterface/elements/textDisplay.h"
#include "../sdlUtils/color.h"
#include "../framework/framework.h"
#include "../framework/colors.h"


class Hud {
    static constexpr int oneSecond {1000};

    static constexpr float healthDisplayX {25};
    static constexpr float healthDisplayY {25};
    static constexpr float healthDisplayGap {10};
    static constexpr int (*healthGetterCb)() {nullptr};
    ImageStatTracker m_playerHealthDisplay{
        healthDisplayX, healthDisplayY, healthDisplayGap,  Framework::images.ballImg, healthGetterCb
    };

    LiveTextDisplay m_scoreDisplay{0, 0, &Framework::fonts.scoreFont, Colors::WHITE, nullptr};

    TextDisplay m_gameOverDisplay{0, 0, "Game Over", &Framework::fonts.scoreFont, Colors::WHITE};
    TextDisplay m_levelClearedDisplay{0, 0, "Level Cleared", &Framework::fonts.scoreFont, Colors::WHITE};


public:
    Hud(Player* playerPtr, int canvasWidth, int canvasHeight);

    void update(SDL_Renderer* renderer);

    void showGameOver(SDL_Renderer* renderer);

    void showLevelCleared(SDL_Renderer* renderer);
};



#endif