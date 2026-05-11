#include <exception>
#include <typeinfo>
#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "./iBlock.h"
#include "./hardBlock.h"
#include "../utils/color.h"
#include "./ball.h"



HardBlock::HardBlock(float x, float y, SDL_Texture* goodImg, SDL_Texture* hitImg, const Color& color, int health, int scoreVal) 
    : m_activeImg{goodImg}, m_goodImg{goodImg}, m_hitImg{hitImg}, m_color{color}, m_health{health}, m_scoreValue{scoreVal} 
{
    m_rect.setTopleft(x, y);
    m_rect.setSize(m_goodImg->w, m_goodImg->h);
};


HardBlock::HardBlock(const IBlock* block) {
    if (typeid(*block) != typeid(HardBlock)) {
        throw std::runtime_error("Wrong underlying block type.\n");
    }

    const HardBlock* realBlockPtr {dynamic_cast<const HardBlock*>(block)};
    m_rect.setTopleft(realBlockPtr->m_rect.topleft());
    m_rect.setSize(realBlockPtr->m_rect.size());

    m_activeImg = realBlockPtr->m_activeImg;
    m_goodImg = realBlockPtr->m_goodImg;
    m_hitImg = realBlockPtr->m_hitImg;

    m_color = realBlockPtr->m_color;

    m_health = realBlockPtr->m_health;
    m_scoreValue = realBlockPtr->m_scoreValue;
};



bool HardBlock::isDead() const {
    return m_health <= 0;
};


void HardBlock::update(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_color.red(), m_color.green(), m_color.blue(), m_color.alpha());
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
    SDL_RenderTexture(renderer, m_activeImg, nullptr, &m_rect.getSDLFRect());
};


void HardBlock::setTopleft(float x, float y) {
    m_rect.setTopleft(x, y);
};


void HardBlock::setColor(const Color& color) {
    m_color = color;
};


const FRect& HardBlock::getRect() const {
    return m_rect;
};


bool HardBlock::hasBallCollision(const Ball& ball) {
    if (!ball.m_hitbox.hasRectIntersection(&m_rect)) {
        return false;
    }

    m_health -= 1;
    m_activeImg = m_hitImg;
    return true;
};