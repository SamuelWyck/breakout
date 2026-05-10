#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H


#include <vector>
#include "../utils/fRect.h"
#include "../entities/ball.h"
#include "../entities/player.h"
#include "../entities/basicBlock.h"
#include "./levelManager.h"


class CollisionManager {
    const FRect& m_screenRect{FRect{}};

    float m_ballSpeedDelta {1.5f};


public:
    CollisionManager(const FRect& screenRect);

    void handleCollisions(Player& player, LevelManager::LevelObjects& levelObjects);


private:
    void handleBallScreenCollisions(Ball& ball);
};



#endif 