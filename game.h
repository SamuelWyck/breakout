#ifndef GAME_H
#define GAME_H


#include "./entities/player.h"
#include "./utils/fRect.h"
#include "./controller/playerController.h"

#include "./entities/ball.h"


class Game {
    FRect m_screenRect {};
    PlayerController m_playerController{};
    Player* m_playerPtr {nullptr};
    Ball* m_ballPtr {nullptr};


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