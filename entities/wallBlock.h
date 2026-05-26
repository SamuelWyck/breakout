#ifndef WALL_BLOCK_H
#define WALL_BLOCK_H


#include <SDL3/SDL_render.h>
#include "./iBlock.h"
#include "../sdlUtils/collision/fRect.h"
#include "../sdlUtils/color.h"
#include "./ball.h"


class WallBlock : public IBlock {
    SDL_Texture* m_img {nullptr};
    Color m_color{};

    FRect m_rect{};


public:
    WallBlock(float x, float y, SDL_Texture* img, const Color& color);

    WallBlock(const IBlock* block);

    WallBlock(const WallBlock&) = default;
    WallBlock(WallBlock&&) = default;
    WallBlock& operator=(const WallBlock&) = default;
    WallBlock& operator=(WallBlock&&) = default;
    ~WallBlock() = default;


    bool isDead() const override;

    void update(SDL_Renderer* renderer) override;

    void setTopleft(float x, float y) override;

    void setColor(const Color& color) override;

    bool hasBallCollision(const Ball& ball) override;

    const FRect& getRect() const override;

    bool isInvincible() const override;

    int getScore() const override;
};



#endif