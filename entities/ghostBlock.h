#ifndef GHOST_BLOCK_H
#define GHOST_BLOCK_H


#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_render.h>
#include "./iBlock.h"
#include "./ball.h"
#include "../sdlUtils/color.h"
#include "../sdlUtils/fRect.h"


class GhostBlock : public IBlock {
    SDL_Surface* m_ghostImgMask {nullptr};
    SDL_Texture* m_ghostImg {nullptr};
    SDL_Texture* m_img {nullptr};
    Color m_color{};

    FRect m_rect{};

    bool m_isGhost {true};
    bool m_deactiveGhost {false};
    bool m_stillHit {false};
    
    int m_health {};
    int m_scoreValue {};


public:
    GhostBlock(float x, float y, SDL_Surface* ghostImgMask, SDL_Texture* img, const Color& color, int health, int scoreVal);

    GhostBlock(const IBlock* block);

    GhostBlock(const GhostBlock&) = delete;
    GhostBlock(GhostBlock&&) = delete;
    GhostBlock& operator=(const GhostBlock&) = delete;
    GhostBlock& operator=(GhostBlock&&) = delete;
    ~GhostBlock();


    bool isDead() const override;

    void update(SDL_Renderer* renderer) override;

    bool hasBallCollision(const Ball& ball) override;

    void setTopleft(float x, float y) override;

    void setColor(const Color& color) override;

    const FRect& getRect() const override;

    bool isInvincible() const override;

    int getScore() const override;


private:
    void createGhostImage();
};



#endif