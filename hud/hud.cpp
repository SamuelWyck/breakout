#include <string>
#include <SDL3/SDL_render.h>
#include "../entities/player.h"
#include "./hud.h"
#include "../framework/framework.h"



Hud::Hud(Player* playerPtr, int canvasWidth, int) {
    m_playerHealthDisplay.setGetterCb(
        [playerPtr]() -> int {
            return playerPtr->getHealth();
        }
    );


    m_scoreDisplay.setTextGetter(
        [playerPtr]() -> std::string {
            return std::to_string(playerPtr->getScore());
        }
    );
    float scoreDisplayX {canvasWidth / 2.0f};
    float scoreDisplayY {1 + healthDisplayY + Framework::images.ballImg->w / 2.0f}; // add one to align with health display
    m_scoreDisplay.setCenter(scoreDisplayX, scoreDisplayY);
};


void Hud::update(SDL_Renderer* renderer) {
    m_playerHealthDisplay.update(renderer);
    m_scoreDisplay.update(renderer);
};