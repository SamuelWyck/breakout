#include <SDL3/SDL_render.h>
#include "../entities/player.h"
#include "./hud.h"



Hud::Hud(Player* playerPtr) {
    auto healthCb {[playerPtr]() -> int {
        return playerPtr->getHealth();
    }};
    m_playerHealthDisplay.setGetterCb(healthCb);
};


void Hud::update(SDL_Renderer* renderer) {
    m_playerHealthDisplay.update(renderer);
};