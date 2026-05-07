#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "./player.h"
#include "./ball.h"
#include "../controller/playerController.h"



Player::Player(float centerX, float centerY, int speed, const PlayerController& controller) 
    : m_speed{speed}, m_controller{controller}
{
    m_rect.setCenter(centerX, centerY);
    m_rect.setWidth(200);
    m_rect.setHeight(30);
};


void Player::update(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect) {
    handleInputs(screenRect, deltaTime);

    if (m_launchBall) {
        m_launchBall = false;
        launchBall();
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
};


void Player::loadBall(Ball* ballPtr) {
    m_ballPtr = ballPtr;
    m_ballPtr->stop();
    positionBall();
};


void Player::positionBall() {
    float ballCenterX {m_rect.centerX()};
    float ballCenterY {m_rect.centerY() - (m_rect.height() / 2) - m_ballPtr->m_hitbox.radius()};
    m_ballPtr->setCenter(ballCenterX, ballCenterY);
};


void Player::launchBall() {
    float ballXSpeedDelta {0.0f};
    if (m_movingLeft && !m_movingRight) {
        ballXSpeedDelta = -1 * m_ballXSpeedDelta;
    } else if (m_movingRight && !m_movingLeft) {
        ballXSpeedDelta = m_ballXSpeedDelta;
    }
    m_ballPtr->launch(ballXSpeedDelta);
    m_ballPtr = nullptr;
};


void Player::handleInputs(const FRect& screenRect, double deltaTime) {
    auto pressedInputs {m_controller.getPressedInputs()};
    m_movingLeft = false;
    m_movingRight = false;


    if (pressedInputs["LEFT"]) {
        m_rect.setX(m_rect.x() - (m_speed * deltaTime));
        m_movingLeft = true;
    }
    if (pressedInputs["RIGHT"]) {
        m_rect.setX(m_rect.x() + (m_speed * deltaTime));
        m_movingRight = true;
    }
    if (pressedInputs["LAUNCH"] && m_ballPtr) {
        m_launchBall = true;
    }

    if (!screenRect.containsRect(&m_rect)) {
        if (m_rect.x() < 0.0f) {
            m_rect.setX(0.0f);
            m_movingLeft = false;
        } else if ((m_rect.x() + m_rect.width()) > screenRect.right()) {
            m_rect.setRight(screenRect.right());
            m_movingRight = false;
        }
    }

    if (m_ballPtr != nullptr) {
        positionBall();
    }
};