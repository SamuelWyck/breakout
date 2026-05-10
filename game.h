#ifndef GAME_H
#define GAME_H


#include "./entities/player.h"
#include "./utils/fRect.h"
#include "./controller/playerController.h"
#include "./gameFramework/collisionManager.h"
#include "./gameFramework/levelManager.h"



class Game {
    FRect m_screenRect {};

    PlayerController m_playerController{};

    CollisionManager* m_collisionManagerPtr{nullptr};
    LevelManager* m_levelManagerPtr{nullptr};

    Player* m_playerPtr {nullptr};


public:
    Game();

    ~Game();

    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;
    Game& operator=(const Game&) = delete;

    void startGame();


private:
    void gameLoop();
};



#endif