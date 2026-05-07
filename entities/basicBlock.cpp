#include <SDL3/SDL_render.h>
#include "./basicBlock.h"



BasicBlock::BasicBlock(float top, float left, const Color& color, int health, int scoreVal) 
    : m_color{color}, m_health{health}, m_scoreValue{scoreVal} 
{
    m_rect.setTopleft(top, left);
    m_rect.setSize(80, 40);
};


bool BasicBlock::isDead() const {
    return m_health <= 0;
};


void BasicBlock::update(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_color.red(), m_color.green(), m_color.blue(), m_color.alpha());
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
};