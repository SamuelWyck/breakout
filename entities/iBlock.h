#ifndef BLOCK_INTERFACE_H
#define BLOCK_INTERFACE_H


#include <SDL3/SDL_render.h>
#include "../utils/color.h"
#include "./ball.h"
#include "../utils/fRect.h"


class IBlock {
public:
    virtual ~IBlock() = default;

    virtual bool isDead() const = 0;

    virtual void update(SDL_Renderer* renderer) = 0;

    virtual void setTopleft(float x, float y) = 0;

    virtual void setColor(const Color& color) = 0;

    virtual bool hasBallCollision(const Ball& ball) = 0;

    virtual const FRect& getRect() const = 0;
};



#endif