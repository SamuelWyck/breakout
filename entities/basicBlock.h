#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H


#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "../utils/color.h"


class BasicBlock {
public:
    FRect m_rect{};


private:
    Color m_color{};

    int m_health {};
    int m_scoreValue {};


public:
    BasicBlock(float top, float left, const Color& color, int health, int scoreVal);

    bool isDead() const;

    void update(SDL_Renderer* renderer);
};



#endif