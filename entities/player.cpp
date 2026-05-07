#include <SDL3/SDL_render.h>
#include "../utils/fRect.h"
#include "../utils/math.h"
#include "./player.h"
#include "./ball.h"
#include "../controller/playerController.h"



Player::Player(float centerX, float centerY, int speed, const PlayerController& controller) 
    : m_speed{speed}, m_controller{controller}
{
    m_rect.setCenter(centerX, centerY);
    m_rect.setWidth(200);
    m_rect.setHeight(30);

    m_bottomRect.setWidth(200);
    m_bottomRect.setHeight(15);
    m_bottomRect.setMidBottom(m_rect.midBottom());
};


void Player::update(SDL_Renderer* renderer, double deltaTime, const FRect& screenRect) {
    handleInputs(screenRect, deltaTime);

    if (m_launchBall) {
        m_launchBall = false;
        launchBall();
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());
    // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    // SDL_RenderFillRect(renderer, &m_bottomRect.getSDLFRect());
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


void Player::handleBallBounce(Ball& ball) {
    if (&ball == m_ballPtr) {
        return;
    }

    if (m_bottomRect.hasRectIntersection(&ball.m_rect)) {
        float xDistance {ball.m_rect.centerX() - m_rect.centerX()};
        if (xDistance < 0.0) {
            float centerX {m_rect.left() - ball.m_hitbox.radius()};
            ball.setCenter(centerX, ball.m_rect.centerY());
            ball.setXSpeed(m_ballXSpeedDelta * -2);
        } else {
            float centerX {m_rect.right() + ball.m_hitbox.radius()};
            ball.setCenter(centerX, ball.m_rect.centerY());
            ball.setXSpeed(m_ballXSpeedDelta * 2);
        }

        return;
    }


    float ballXSpeedDelta {0.0f};
    if (m_movingLeft && !m_movingRight) {
        ballXSpeedDelta -= m_ballXSpeedDelta;
    } else if (m_movingRight && !m_movingLeft) {
        ballXSpeedDelta += m_ballXSpeedDelta;
    }

    constexpr float ballYSpeedDelta {0.0f};
    ball.bounceY(ballYSpeedDelta);
    ball.changeXSpeed(ballXSpeedDelta);
    ball.setCenter(ball.m_rect.centerX(), m_rect.top() - ball.m_hitbox.radius());
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

    m_bottomRect.setMidBottom(m_rect.midBottom());
    if (m_ballPtr != nullptr) {
        positionBall();
    }
};