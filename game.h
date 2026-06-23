#ifndef GAME_H
#define GAME_H


#include "./entities/player.h"
#include "./sdlUtils/collision/fRect.h"
#include "./sdlUtils/playerController/playerController.h"
#include "./gameFramework/collisionManager.h"
#include "./gameFramework/levelManager.h"
#include "./hud/hud.h"
#include "./sdlUtils/userInterface/mouse.h"
#include "./gameFramework/ui.h"


class Game {
    FRect m_screenRect {};

    PlayerController m_playerController{};

    CollisionManager* m_collisionManagerPtr{nullptr};
    LevelManager* m_levelManagerPtr{nullptr};

    Player* m_playerPtr {nullptr};

    Hud* m_hudPtr {nullptr};

    Mouse m_mouseManager{};

    Ui* m_ui {nullptr};


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