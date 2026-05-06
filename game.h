#ifndef GAME_H
#define GAME_H


#include "./entities/player.h"
#include "./utils/fRect.h"


class Game {
    Player* m_playerPtr {nullptr};
    FRect m_screenRect {};
    

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