#ifndef PLAYER_H
#define PLAYER_H


#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "../controller/playerController.h"
#include "./ball.h"


class Player {
    FRect m_rect{};

    int m_speed {};
    bool m_movingLeft {false};
    bool m_movingRight {false};
    
    const PlayerController& m_controller{};

    bool m_launchBall {false};
    Ball* m_ballPtr {nullptr};
    float m_ballXSpeedDelta {3.0f};


public:
    Player(float centerX, float centerY, int speed, const PlayerController& controller);

    void update(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect);

    void loadBall(Ball* ballPtr);


private:
    void handleInputs(const FRect& screenRect, double deltaTime);

    void positionBall();

    void launchBall();
};



#endif