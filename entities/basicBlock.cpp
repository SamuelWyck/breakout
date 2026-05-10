#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "./basicBlock.h"
#include "./ball.h"



BasicBlock::BasicBlock(float x, float y, SDL_Texture* img, const Color& color, int health, int scoreVal) 
    : m_color{color}, m_img{img}, m_health{health}, m_scoreValue{scoreVal} 
{
    m_rect.setTopleft(x, y);
    m_rect.setSize(96, 36);
};


BasicBlock::BasicBlock() {
};


bool BasicBlock::isDead() const {
    return m_health <= 0;
};


void BasicBlock::setTopLeft(float x, float y) {
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