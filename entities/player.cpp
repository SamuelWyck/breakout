#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "./player.h"
#include "../controller/playerController.h"



Player::Player(float centerX, float centerY, int speed, const PlayerController& controller) 
    : m_speed{speed}, m_controller{controller}
{
    m_rect.setCenter(centerX, centerY);
    m_rect.setWidth(200);
    m_rect.setHeight(30);
};


void Player::update(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect) {
    handleMovement(screenRect, deltaTime);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
};


void Player::handleMovement(const FRect& screenRect, double deltaTime) {
    auto pressedInputs {m_controller.getPressedInputs()};


    if (pressedInputs["LEFT"]) {
        m_rect.setX(m_rect.x() - (m_speed * deltaTime));
    }
    if (pressedInputs["RIGHT"]) {
        m_rect.setX(m_rect.x() + (m_speed * deltaTime));
    }

    if (!screenRect.containsRect(&m_rect)) {
        if (m_rect.x() < 0.0f) {
            m_rect.setX(0.0f);
        } else if ((m_rect.x() + m_rect.width()) > screenRect.right()) {
            m_rect.setRight(screenRect.right());
        }
    }
};