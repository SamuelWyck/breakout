#include <string>
#include <SDL3/SDL_render.h>
#include "../entities/player.h"
#include "./hud.h"
#include "../framework/framework.h"



Hud::Hud(Player* playerPtr, int canvasWidth, int) {
    auto healthCb {[playerPtr]() -> int {
        return playerPtr->getHealth();
    }};
    m_playerHealthDisplay.setGetterCb(healthCb);


    float scoreDisplayX {canvasWidth / 2.0f};
    float scoreDisplayY {30};
    m_scoreDisplay.setCenter(scoreDisplayX, scoreDisplayY);
    m_scoreDisplay.setTextGetter(
        [playerPtr]() -> std::string {
            return std::to_string(playerPtr->getHealth());
        }
    );
};


void Hud::update(SDL_Renderer* renderer) {
    m_playerHealthDisplay.update(renderer);
    m_scoreDisplay.update(renderer);
};