#include <unordered_map>
#include <unordered_set>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "./game.h"
#include "./framework/framework.h"
#include "./utils/clock.h"
#include "./entities/player.h"
#include "./controller/playerController.h"
#include "./gameFramework/collisionManager.h"
#include "./controller/userInput.h"
#include "./utils/fRect.h"



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
        playerSpeed,
        m_playerController
    };

    constexpr float ballXSpeed {0};
    constexpr float ballYSpeed {0};
    constexpr float ballRadius {15};
    constexpr float ballCenterCoord {0};
    m_ballPtr = new Ball{ballCenterCoord, ballCenterCoord, ballRadius, ballXSpeed, ballYSpeed};

    m_playerPtr->loadBall(m_ballPtr);


    // set up collision manager
    m_collisionManagerPtr = new CollisionManager{m_screenRect};
};


Game::~Game() {
    delete m_playerPtr;
    delete m_ballPtr;
    delete m_collisionManagerPtr;
};


void Game::startGame() {
    gameLoop();
};


void Game::gameLoop() {
    bool running {true};
    SDL_Texture* img {IMG_LoadTexture(Framework::display.renderer(), "C:\\Users\\samaw\\.vscode\\breakout\\wall2.png")};
    SDL_SetTextureScaleMode(img, SDL_SCALEMODE_PIXELART);
    if (!img) {
        SDL_Log(SDL_GetError());
    }

    FRect realRect{500, 500, 40, 40};

    Clock clock{};

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


        m_collisionManagerPtr->handleCollisions(*m_playerPtr, *m_ballPtr);

        realRect.pivot(450, 450, deltaTime * 2);

        SDL_SetRenderDrawColor(Framework::display.renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Framework::display.renderer());

        SDL_RenderTexture(Framework::display.renderer(), img, nullptr, &realRect.getSDLFRect());

        m_playerPtr->update(Framework::display.renderer(), deltaTime, m_screenRect);
        m_ballPtr->update(Framework::display.renderer(), deltaTime);
        // SDL_RenderTextureRotated(Framework::display.renderer(), img, nullptr, &srcRect, angle, nullptr, SDL_FLIP_NONE);
        
        SDL_RenderPresent(Framework::display.renderer());
        m_playerController.resetPressedInputs();
    }
};