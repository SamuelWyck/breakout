#ifndef BALL_BLOCK_H
#define BALL_BLOCK_H


#include <vector>
#include <SDL3/SDL_render.h>
#include "../sdlUtils/collision/fRect.h"
#include "../sdlUtils/color.h"
#include "./iBlock.h"
#include "./ball.h"


class BallBlock : public IBlock {
    SDL_Texture* m_img {nullptr};
    Color m_color{};

    FRect m_rect{};

    int m_health {};
    int m_scoreValue {};
    
    std::vector<Ball*>* m_ballsPtr {nullptr};

public:
    BallBlock(float x, float y, SDL_Texture* img, const Color& color, int health, int scoreVal, std::vector<Ball*>* balls);

    BallBlock(const IBlock* block);

    BallBlock(const BallBlock&) = default;
    BallBlock(BallBlock&&) = default;
    BallBlock& operator=(const BallBlock&) = default;
    BallBlock& operator=(BallBlock&&) = default;
    ~BallBlock() = default;


    bool isDead() const override;

    const FRect& getRect() const override;

    bool hasBallCollision(const Ball& ball) override;

    void setTopleft(float x, float y) override;

    void setColor(const Color& color) override;

    void update(SDL_Renderer* renderer) override;

    bool isInvincible() const override;

    int getScore() const override;
};



#endif