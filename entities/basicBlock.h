#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H


#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "../utils/color.h"
#include "./ball.h"


class BasicBlock {
public:
    FRect m_rect{};


private:
    Color m_color{};

    int m_health {};
    int m_scoreValue {};


public:
    BasicBlock(float x, float y, const Color& color, int health, int scoreVal);
    BasicBlock();

    bool isDead() const;

    void setTopLeft(float x, float y);

    void setColor(const Color& color);

    void update(SDL_Renderer* renderer);

    bool hasBallCollision(const Ball& ball);
};



#endif