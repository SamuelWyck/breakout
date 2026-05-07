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

    if (m_ySpeed > m_maxSpeedPos) {
        m_ySpeed = m_maxSpeedPos;
    } else if (m_ySpeed < m_maxSpeedNeg) {
        m_ySpeed = m_maxSpeedNeg;
    }
};


void Ball::bounceX(float xSpeedDelta) {
    m_xSpeed *= -1.0f;
    if (xSpeedDelta < 0.0) {
        m_xSpeed = (m_xSpeed < 0.0) ? m_xSpeed + xSpeedDelta : m_xSpeed - xSpeedDelta;
    } else {
        m_xSpeed = (m_xSpeed < 0.0) ? m_xSpeed - xSpeedDelta : m_xSpeed + xSpeedDelta;
    }

    if (m_xSpeed > m_maxSpeedPos) {
        m_xSpeed = m_maxSpeedPos;
    } else if (m_xSpeed < m_maxSpeedNeg) {
        m_xSpeed = m_maxSpeedNeg;
    }
};


void Ball::changeXSpeed(float deltaSpeed) {
    m_xSpeed += deltaSpeed;
    if (m_xSpeed > m_maxSpeedPos) {
        m_xSpeed = m_maxSpeedPos;
    } else if (m_xSpeed < m_maxSpeedNeg) {
        m_xSpeed = m_maxSpeedNeg;
    }
};


void Ball::changeYSpeed(float deltaSpeed) {
    m_ySpeed += deltaSpeed;
    if (m_ySpeed > m_maxSpeedPos) {
        m_ySpeed = m_maxSpeedPos;
    } else if (m_ySpeed < m_maxSpeedNeg) {
        m_ySpeed = m_maxSpeedNeg;
    }
};


void Ball::setXSpeed(float xSpeed) {
    m_xSpeed = xSpeed;
    if (m_xSpeed > m_maxSpeedPos) {
        m_xSpeed = m_maxSpeedPos;
    } else if (m_xSpeed < m_maxSpeedNeg) {
        m_xSpeed = m_maxSpeedNeg;
    }
};


void Ball::setYSpeed(float ySpeed) {
    m_ySpeed = ySpeed;
    if (m_ySpeed > m_maxSpeedPos) {
        m_ySpeed = m_maxSpeedPos;
    } else if (m_ySpeed < m_maxSpeedNeg) {
        m_ySpeed = m_maxSpeedNeg;
    }
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
        bounceX(speedDelta);
        setCenter(blockRect.right() + m_hitbox.radius(), m_rect.centerY());

    } else if (minDistance == rightSideDis) {
        bounceX(speedDelta);
        setCenter(blockRect.left() - m_hitbox.radius(), m_rect.centerY());

    } else if (minDistance == bottomSideDis) {
        bounceY(speedDelta);
        setCenter(m_rect.centerX(), blockRect.top() - m_hitbox.radius());

    } else if (minDistance == topSideDis) {
        bounceY(speedDelta);
        setCenter(m_rect.centerX(), blockRect.bottom() + m_hitbox.radius());
    }
};