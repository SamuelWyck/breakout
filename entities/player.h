#ifndef PLAYER_H
#define PLAYER_H


#include <SDL3/SDL_render.h>
#include "../sdlUtils/fRect.h"
#include "../controller/playerController.h"
#include "./ball.h"


class Player {
public:
    FRect m_rect{};


private:
    float m_startCenterX {};
    float m_startCenterY {};

    FRect m_bottomRect{};
    SDL_Texture* m_img {nullptr};

    int m_speed {};
    bool m_movingLeft {false};
    bool m_movingRight {false};
    
    PlayerController* m_controller{nullptr};

    bool m_launchBall {false};
    Ball* m_ballPtr {nullptr};
    float m_ballXSpeedDelta {2.0f};

    int m_health {3};
    int m_maxHealth {3};



public:
    Player(float centerX, float centerY, SDL_Texture* img, int speed, PlayerController* controller);

    void update(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect);

    void loadBall(Ball* ballPtr);

    void handleBallBounce(Ball& ball);

    void launchBall();

    int getHealth() const;

    void takeDamage();

    void healHealth();

    bool isDead() const;

    void reset();

    void resetPosition();


private:
    void handleInputs(const FRect& screenRect, double deltaTime);

    void positionBall();
};



#endif