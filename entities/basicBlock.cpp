#include <typeinfo>
#include <exception>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "./basicBlock.h"
#include "../sdlUtils/color.h"
#include "./ball.h"
#include "../sdlUtils/fRect.h"


BasicBlock::BasicBlock(float x, float y, SDL_Texture* img, const Color& color, int health, int scoreVal) 
    : m_color{color}, m_img{img}, m_health{health}, m_scoreValue{scoreVal} 
{
    m_rect.setTopleft(x, y);
    m_rect.setSize(m_img->w, m_img->h);
};


BasicBlock::BasicBlock() {
};


BasicBlock::BasicBlock(const IBlock* block) {
    if (typeid(*block) != typeid(BasicBlock)) {
        throw std::runtime_error("Wrong underlying block type.\n");
    }

    const BasicBlock* realBlockPtr {dynamic_cast<const BasicBlock*>(block)};

    m_rect.setTopleft(realBlockPtr->m_rect.topleft());
    m_rect.setSize(realBlockPtr->m_rect.size());
    
    m_color = realBlockPtr->m_color;
    m_health = realBlockPtr->m_health;
    m_scoreValue = realBlockPtr->m_scoreValue;
    m_img = realBlockPtr->m_img;
};


bool BasicBlock::isDead() const {
    return m_health <= 0;
};


void BasicBlock::setTopleft(float x, float y) {
    m_rect.setTopleft(x, y);
};


void BasicBlock::setColor(const Color& color) {
    m_color = color;
};


void BasicBlock::update(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_color.red(), m_color.green(), m_color.blue(), m_color.alpha());
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
    SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
};


bool BasicBlock::hasBallCollision(const Ball& ball) {
    if (!ball.m_hitbox.hasRectIntersection(&m_rect.getSDLFRect())) {
        return false;
    }

    m_health -= 1;
    return true;
};


const FRect& BasicBlock::getRect() const {
    return m_rect;
};


bool BasicBlock::isInvincible() const {
    return false;
};