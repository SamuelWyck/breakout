#include <utility>
#include <SDL3/SDL_render.h>
#include "./ball.h"



Ball::Ball(SDL_Texture* img, float centerX, float centerY, float radius, float xSpeed, float ySpeed) 
    : m_hitbox{centerX, centerY, radius}, m_img{img}, m_xSpeed{xSpeed}, m_ySpeed{ySpeed} 
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

    SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
};


void Ball::launch(float xSpeedDelta) {
    m_ySpeed = m_maxSpeedNeg;
    m_xSpeed += xSpeedDelta;
};


void Ball::stop() {
    m_xSpeed = 0.0f;
    m_ySpeed = 0.0f;
};


void Ball::bounceY(float ySpeedDelta) {
    m_ySpeed *= -1.0f;
    if (ySpeedDelta < 0.0) {
        m_ySpeed = (m_ySpeed < 0.0) ? m_ySpeed + ySpeedDelta : m_ySpeed - ySpeedDelta;
    } else {
        m_ySpeed = (m_ySpeed < 0.0) ? m_ySpeed - ySpeedDelta : m_ySpeed + ySpeedDelta;
    }

    m_ySpeed = clampSpeed(m_ySpeed);
};


void Ball::bounceX(float xSpeedDelta) {
    m_xSpeed *= -1.0f;
    if (xSpeedDelta < 0.0) {
        m_xSpeed = (m_xSpeed < 0.0) ? m_xSpeed + xSpeedDelta : m_xSpeed - xSpeedDelta;
    } else {
        m_xSpeed = (m_xSpeed < 0.0) ? m_xSpeed - xSpeedDelta : m_xSpeed + xSpeedDelta;
    }

    m_xSpeed = clampSpeed(m_xSpeed);
};


void Ball::changeXSpeed(float deltaSpeed) {
    m_xSpeed += deltaSpeed;
    m_xSpeed = clampSpeed(m_xSpeed);
};


void Ball::changeYSpeed(float deltaSpeed) {
    m_ySpeed += deltaSpeed;
    m_ySpeed = clampSpeed(m_ySpeed);
};


float Ball::xSpeed() const {
    return m_xSpeed;
};


void Ball::setXSpeed(float xSpeed) {
    m_xSpeed = clampSpeed(xSpeed);
};


float Ball::ySpeed() const {
    return m_ySpeed;
};


void Ball::setYSpeed(float ySpeed) {
    m_ySpeed = clampSpeed(ySpeed);
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


void Ball::handleBlockCollision(const FRect& blockRect) {
    float leftSideDis {m_rect.left() - blockRect.right()};
    leftSideDis = (leftSideDis < 0.0f) ? -1 * leftSideDis : leftSideDis;

    float rightSideDis {m_rect.right() - blockRect.left()};
    rightSideDis = (rightSideDis < 0.0f) ? -1 * rightSideDis : rightSideDis;

    float bottomSideDis {m_rect.bottom() - blockRect.top()};
    bottomSideDis = (bottomSideDis < 0.0f) ? -1 * bottomSideDis : bottomSideDis;

    float topSideDis {m_rect.top() - blockRect.bottom()};
    topSideDis = (topSideDis < 0.0f) ? -1 * topSideDis : topSideDis;

    float minDistance {leftSideDis};
    minDistance = (minDistance > rightSideDis) ? rightSideDis : minDistance;
    minDistance = (minDistance > bottomSideDis) ? bottomSideDis : minDistance;
    minDistance = (minDistance > topSideDis) ? topSideDis : minDistance;


    constexpr float speedDelta {1.0f};
    if (minDistance == leftSideDis) {
        if (m_xSpeed < 0.0f) {
            bounceX(speedDelta);
        }
        setCenter(blockRect.right() + m_hitbox.radius(), m_rect.centerY());

    } else if (minDistance == rightSideDis) {
        if (m_xSpeed > 0.0f) {
            bounceX(speedDelta);
        }
        setCenter(blockRect.left() - m_hitbox.radius(), m_rect.centerY());

    } else if (minDistance == bottomSideDis) {
        if (m_ySpeed > 0.0f) {
            bounceY(speedDelta);
        }
        setCenter(m_rect.centerX(), blockRect.top() - m_hitbox.radius());

    } else if (minDistance == topSideDis) {
        if (m_ySpeed < 0.0f) {
            bounceY(speedDelta);
        }
        setCenter(m_rect.centerX(), blockRect.bottom() + m_hitbox.radius());
    }
};


void Ball::handleBallCollision(Ball& ball) {
    handleBlockCollision(ball.m_rect);
    ball.handleBlockCollision(m_rect);
};


float Ball::clampSpeed(float speed) {
    if (speed > m_maxSpeedPos) {
        speed = m_maxSpeedPos;
    } else if (speed < m_maxSpeedNeg) {
        speed = m_maxSpeedNeg;
    }
    return speed;
};