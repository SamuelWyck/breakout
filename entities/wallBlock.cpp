#include <exception>
#include <typeinfo>
#include <SDL3/SDL_render.h>
#include "./iBlock.h"
#include "../utils/fRect.h"
#include "../utils/color.h"
#include "./wallBlock.h"
#include "./ball.h"



WallBlock::WallBlock(float x, float y, SDL_Texture* img, const Color& color) 
    : m_img{img}, m_color{color} 
{
    m_rect.setTopleft(x, y);
    m_rect.setSize(m_img->w, m_img->h);
}


WallBlock::WallBlock(const IBlock* block) {
    if (typeid(*block) != typeid(WallBlock)) {
        throw std::runtime_error("Wrong underlying block type.\n");
    }

    const WallBlock* realBlockPtr {dynamic_cast<const WallBlock*>(block)};
    m_rect.setTopleft(realBlockPtr->m_rect.topleft());
    m_rect.setSize(realBlockPtr->m_rect.size());

    m_color = realBlockPtr->m_color;
    m_img = realBlockPtr->m_img;
};



bool WallBlock::isDead() const {
    return false;
};


void WallBlock::update(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_color.red(), m_color.green(), m_color.blue(), m_color.alpha());
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
    SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
};


void WallBlock::setTopleft(float x, float y) {
    m_rect.setTopleft(x, y);
};


void WallBlock::setColor(const Color& color) {
    m_color = color;
};


bool WallBlock::hasBallCollision(const Ball& ball) {
    return ball.m_hitbox.hasRectIntersection(&m_rect);
};


const FRect& WallBlock::getRect() const {
    return m_rect;
};


bool WallBlock::isInvincible() const {
    return true;
};