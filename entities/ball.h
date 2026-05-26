#ifndef BALL_H
#define BALL_H


#include <utility>
#include <SDL3/SDL_render.h>
#include "../sdlUtils/collision/fRect.h"
#include "../sdlUtils/collision/fCircle.h"
#include "../sdlUtils/audio/soundEffect.h"


class Ball {
public:
    FRect m_rect{};
    FCircle m_hitbox{};
    
    
private:
    SDL_Texture* m_img {nullptr};
    
    float m_xSpeed {0.0f};
    float m_ySpeed {0.0f};
    float m_maxSpeedPos {5.5f};
    float m_maxSpeedNeg {-5.5f};
    
    SoundEffect m_sound{};


public:
    Ball(SDL_Texture* img, float centerX, float centerY, float radius, float xSpeed, float ySpeed);
    Ball();


    void update(SDL_Renderer* renderer, double deltaTime);

    
    void launch(float xSpeedDelta);
    void stop();


    void bounceY(float ySpeedDelta);
    void bounceX(float xSpeedDelta);


    void changeXSpeed(float deltaSpeed);
    void changeYSpeed(float deltaSpeed);

    
    float xSpeed() const;
    void setXSpeed(float xSpeed);
    float ySpeed() const;
    void setYSpeed(float ySpeed);


    void setCenter(float centerX, float centerY);
    void setCenter(const std::pair<float, float>& center);


    void handleBlockCollision(const FRect& blockRect);
    void handleBallCollision(Ball& ball);


    void playBounceSound() const;


private:
    float clampSpeed(float speed);
};



#endif