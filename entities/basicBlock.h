#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H


#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "../sdlUtils/fRect.h"
#include "../sdlUtils/color.h"
#include "./ball.h"
#include "./iBlock.h"


class BasicBlock : public IBlock {
private:
    FRect m_rect{};
    Color m_color{};
    SDL_Texture* m_img {nullptr};

    int m_health {};
    int m_scoreValue {};


public:
    BasicBlock(float x, float y, SDL_Texture* img, const Color& color, int health, int scoreVal);
    BasicBlock();

    ~BasicBlock() = default;
    BasicBlock(const BasicBlock&) = default;
    BasicBlock(const IBlock* block);
    BasicBlock(BasicBlock&&) = default;
    BasicBlock& operator=(const BasicBlock&) = default;
    BasicBlock& operator=(BasicBlock&&) = default;

    bool isDead() const override;

    void setTopleft(float x, float y) override;

    void setColor(const Color& color) override;

    void update(SDL_Renderer* renderer) override;

    bool hasBallCollision(const Ball& ball) override;

    const FRect& getRect() const override;

    bool isInvincible() const override;
};



#endif