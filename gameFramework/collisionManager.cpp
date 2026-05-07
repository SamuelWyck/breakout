#include <vector>
#include "./collisionManager.h"
#include "../entities/ball.h"
#include "../entities/player.h"
#include "../entities/basicBlock.h"



CollisionManager::CollisionManager(const FRect& screenRect) 
    : m_screenRect{screenRect} {
};


void CollisionManager::handleCollisions(Player& player, Ball& ball, const std::vector<BasicBlock*>& blocks) {
    handleBallScreenCollisions(ball);

    if (player.m_rect.hasCircleIntersection(&ball.m_hitbox)) {
        player.handleBallBounce(ball);
    }

    for (BasicBlock* blockPtr : blocks) {
        if (blockPtr->hasBallCollision(ball)) {
            ball.handleBlockCollision(blockPtr->m_rect);
        }
    }
};


void CollisionManager::handleBallScreenCollisions(Ball& ball) {
    if (m_screenRect.containsRect(&ball.m_rect)) {
        return;
    }

    if (ball.m_rect.top() < m_screenRect.top()) {
        ball.m_rect.setTop(m_screenRect.top());
        ball.bounceY(m_ballSpeedDelta);

    }

    if (ball.m_rect.left() < m_screenRect.left()) {
        ball.m_rect.setLeft(m_screenRect.left());
        ball.bounceX(m_ballSpeedDelta);

    } else if (ball.m_rect.right() > m_screenRect.right()) {
        ball.m_rect.setRight(m_screenRect.right());
        ball.bounceX(m_ballSpeedDelta);
    }
};