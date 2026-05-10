#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H


#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "../utils/fRect.h"
#include "../utils/color.h"
#include "./ball.h"


class BasicBlock {
public:
    FRect m_rect{};


private:
    Color m_color{};
    SDL_Texture* m_img {nullptr};

    int m_health {};
    int m_scoreValue {};


public:
    BasicBlock(float x, float y, SDL_Texture* img, const Color& color, int health, int scoreVal);
    BasicBlock();

    ~BasicBlock() = default;
    BasicBlock(const BasicBlock&) = default;
    BasicBlock(BasicBlock&&) = default;
    BasicBlock& operator=(const BasicBlock&) = default;
    BasicBlock& operator=(BasicBlock&&) = default;

    bool isDead() const;

    void setTopLeft(float x, float y);

    void setColor(const Color& color);

    void update(SDL_Renderer* renderer);

    bool hasBallCollision(const Ball& ball);
};



#endif