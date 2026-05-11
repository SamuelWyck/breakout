#ifndef HARD_BLOCK_H
#define HARD_BLOCK_H



#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "./iBlock.h"
#include "../utils/color.h"
#include "./ball.h"



class HardBlock : public IBlock {
    SDL_Texture* m_activeImg {nullptr};
    SDL_Texture* m_goodImg {nullptr};
    SDL_Texture* m_hitImg {nullptr};

    Color m_color{};

    FRect m_rect{};
    int m_health {};
    int m_scoreValue {};

    bool m_justHit {false};
    bool m_stillHit {false};


public:
    HardBlock(float x, float y, SDL_Texture* goodImg, SDL_Texture* hitImg, const Color& color, int health, int scoreVal);

    HardBlock(const IBlock* block);

    HardBlock(const HardBlock&) = default;
    HardBlock(HardBlock&&) = default;
    HardBlock& operator=(const HardBlock&) = default;
    HardBlock& operator=(HardBlock&&) = default;
    ~HardBlock() = default;


    bool isDead() const override;

    void update(SDL_Renderer* renderer) override;

    const FRect& getRect() const override;

    bool hasBallCollision(const Ball& ball) override;

    void setTopleft(float x, float y) override;

    void setColor(const Color& color) override;
};



#endif