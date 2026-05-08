#include <vector>
#include <utility>
#include "./collisionManager.h"
#include "../entities/ball.h"
#include "../entities/player.h"
#include "../entities/basicBlock.h"
#include "./levelManager.h"



CollisionManager::CollisionManager(const FRect& screenRect) 
    : m_screenRect{screenRect} {
};


void CollisionManager::handleCollisions(Player& player, LevelManager::LevelObjects& levelObjects) {
    std::vector<Ball*>& balls {levelObjects.first};
    std::vector<BasicBlock*>& blocks{levelObjects.second};

    for (Ball* ballPtr : balls) {
        handleBallScreenCollisions(*ballPtr);

        if (player.m_rect.hasCircleIntersection(&ballPtr->m_hitbox)) {
            player.handleBallBounce(*ballPtr);
        }


        for (BasicBlock* blockPtr : blocks) {
            if (blockPtr->isDead()) {
                continue;
            }

            if (blockPtr->hasBallCollision(*ballPtr)) {
                ballPtr->handleBlockCollision(blockPtr->m_rect);
            }
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