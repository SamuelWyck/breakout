#include <utility>
#include <SDL3/SDL_render.h>
#include "./ball.h"



Ball::Ball(float centerX, float centerY, float radius, float xSpeed, float ySpeed) 
    : m_hitbox{centerX, centerY, radius}, m_xSpeed{xSpeed}, m_ySpeed{ySpeed} 
{
    m_rect.setCenter(centerX, centerY);
    m_rect.setSize(radius * 2, radius * 2);    
};

Ball::Ball() {
};


void Ball::update(SDL_Renderer* renderer, double deltaTime) {
    float realXSpeed {m_xSpeed * static_cast<float>(deltaTime)};
    float realYSpeed {m_ySpeed * static_cast<float>(deltaTime)};

    m_rect.setX(m_rect.x() + realXSpeed);
    m_rect.setY(m_rect.y() + realYSpeed);
    m_hitbox.setCenter(m_rect.center());

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
};


void Ball::launch(float xSpeedDelta) {
    m_ySpeed = -3.0f;
    m_xSpeed += xSpeedDelta;
};


void Ball::stop() {
    m_xSpeed = 0.0f;
    m_ySpeed = 0.0f;
};


void Ball::setCenter(float centerX, float centerY) {
    m_rect.setCenter(centerX, centerY);
    m_hitbox.setCenter(centerX, centerY);
};

void Ball::setCenter(const std::pair<float, float>& center) {
    auto [centerX, centerY] = center;
    m_rect.setCenter(centerX, centerY);
    m_hitbox.setCenter(centerX, centerY);
};