#ifndef PLAYER_H
#define PLAYER_H


#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "../controller/playerController.h"


class Player {
    FRect m_rect{};
    int m_speed {};

    const PlayerController& m_controller{};


public:
    Player(float centerX, float centerY, int speed, const PlayerController& controller);

    void update(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect);


private:
    void handleMovement(const FRect& screenRect, double deltaTime);
};



#endif