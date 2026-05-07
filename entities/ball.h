#ifndef BALL_H
#define BALL_H


#include <utility>
#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "../utils/fCircle.h"


class Ball {
public:
    FRect m_rect{};
    FCircle m_hitbox{};


private:
    float m_xSpeed {0.0f};
    float m_ySpeed {0.0f};
    float m_maxSpeedPos {5.5f};
    float m_maxSpeedNeg {-5.5f};


public:
    Ball(float centerX, float centerY, float radius, float xSpeed, float ySpeed);
    Ball();


    void update(SDL_Renderer* renderer, double deltaTime);

    
    void launch(float xSpeedDelta);
    void stop();


    void bounceY(float ySpeedDelta);
    void bounceX(float xSpeedDelta);


    void changeXSpeed(float deltaSpeed);
    void changeYSpeed(float deltaSpeed);

    
    void setXSpeed(float xSpeed);
    void setYSpeed(float ySpeed);


    void setCenter(float centerX, float centerY);
    void setCenter(const std::pair<float, float>& center);


    void handleBlockCollision(const FRect& blockRect);
};



#endif