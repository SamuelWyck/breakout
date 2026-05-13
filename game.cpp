#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include "./game.h"
#include "./framework/framework.h"
#include "./sdlUtils/clock.h"
#include "./entities/player.h"
#include "./controller/playerController.h"
#include "./gameFramework/collisionManager.h"
#include "./gameFramework/levelManager.h"
#include "./controller/userInput.h"
#include "./hud/hud.h"



Game::Game() 
: m_screenRect{
    0, 0, 
    static_cast<float>(Framework::display.canvasWidth()), 
    static_cast<float>(Framework::display.canvasHeight())
}, 
m_playerController{
    std::unordered_map<std::string, UserInput>{
        {"LEFT", UserInput{SDL_SCANCODE_A}}, 
        {"RIGHT", UserInput{SDL_SCANCODE_D}},
        {"LAUNCH", UserInput{SDL_SCANCODE_SPACE}}
    },
    std::unordered_set<std::string>{"LEFT", "RIGHT"}
} {

    constexpr int playerSpeed {9};
    m_playerPtr = new Player{
        Framework::display.canvasWidth() / 2.0f, 
        Framework::display.canvasHeight() - 50.0f,
        Framework::images.playerImg,
        playerSpeed,
        &m_playerController
    };


    // set up hud manager
    m_hudPtr = new Hud{m_playerPtr};


    // set up collision manager
    m_collisionManagerPtr = new CollisionManager{m_screenRect};


    // set up level manager
    constexpr int levelX {118};
    constexpr int levelY {80};
    constexpr int tileWidth {100};
    constexpr int tileHeight {40};
    constexpr int blockWidth {96};
    constexpr int blockHeight {36};
    m_levelManagerPtr = new LevelManager{levelX, levelY, tileWidth, tileHeight, blockWidth, blockHeight, m_playerPtr};
};


Game::~Game() {
    delete m_collisionManagerPtr;
    delete m_levelManagerPtr;
    delete m_hudPtr;
    delete m_playerPtr;
};


void Game::startGame() {
    m_levelManagerPtr->loadLevel(0);
    gameLoop();
};


void Game::gameLoop() {
    bool running {true};
    Clock clock{};

    constexpr int firstLevel {0};
    constexpr int levelCleared {1};
    constexpr int levelFailed {-1};

    while (running) {
        double deltaTime {clock.getNormalizedDeltaTime()};

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;

            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.scancode == SDL_SCANCODE_BACKSPACE) {
                    running = false;
                } else {
                    m_playerController.handleKeyDown(event.key.scancode);
                }

            } else if (event.type == SDL_EVENT_KEY_UP) {
                m_playerController.handleKeyUp(event.key.scancode);

            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                m_playerController.handleMouseDown(event.button.button);

            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                m_playerController.handleMouseUp(event.button.button);

            }
        }


        LevelManager::LevelObjects levelObjects{m_levelManagerPtr->getLevelObjects()};
        int gameStatus {m_collisionManagerPtr->handleCollisions(*m_playerPtr, levelObjects)};
        if (gameStatus == levelFailed) {
            m_playerPtr->takeDamage();
            if (m_playerPtr->isDead()) {
                m_levelManagerPtr->loadLevel(firstLevel);
                m_playerPtr->reset();
                continue;
            } else {
                m_levelManagerPtr->loadPlayerPaddle();
            }

        } else if (gameStatus == levelCleared) {
            m_levelManagerPtr->loadLevel(m_levelManagerPtr->getLevelNum() + 1);
            continue;
        }


        SDL_SetRenderDrawColor(Framework::display.renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Framework::display.renderer());
        SDL_RenderTexture(
            Framework::display.renderer(), 
            Framework::images.backgroundImg, 
            nullptr, &m_screenRect.getSDLFRect()
        );

        m_hudPtr->update(Framework::display.renderer());
        m_levelManagerPtr->updateLevel(Framework::display.renderer(), deltaTime, m_screenRect);
        m_playerPtr->update(Framework::display.renderer(), deltaTime, m_screenRect);
        
        SDL_RenderPresent(Framework::display.renderer());
        m_playerController.resetPressedInputs();
    }
};