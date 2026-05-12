#include <vector>
#include <utility>
#include "./collisionManager.h"
#include "../entities/ball.h"
#include "../entities/player.h"
#include "../entities/basicBlock.h"
#include "../entities/iBlock.h"
#include "./levelManager.h"



CollisionManager::CollisionManager(const FRect& screenRect) 
    : m_screenRect{screenRect} {
};


void CollisionManager::handleCollisions(Player& player, LevelManager::LevelObjects& levelObjects) {
    std::vector<Ball*>& balls {levelObjects.first};
    std::vector<IBlock*>& blocks{levelObjects.second};

    int length {static_cast<int>(balls.size())};
    for (int i {0}; i < length; i += 1) {
        Ball* ballPtr {balls[i]};
        
        handleBallScreenCollisions(*ballPtr);

        if (player.m_rect.hasCircleIntersection(&ballPtr->m_hitbox)) {
            player.handleBallBounce(*ballPtr);
        }


        for (IBlock* blockPtr : blocks) {
            if (blockPtr->isDead()) {
                continue;
            }

            if (blockPtr->hasBallCollision(*ballPtr)) {
                ballPtr->handleBlockCollision(blockPtr->getRect());
            }
        }


        for (Ball* otherBallPtr : balls) {
            if (ballPtr == otherBallPtr) {
                continue;
            }

            if (ballPtr->m_hitbox.hasCircleIntersection(&otherBallPtr->m_hitbox)) {
                ballPtr->handleBallCollision(*otherBallPtr);
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