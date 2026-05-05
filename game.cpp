#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "./game.h"
#include "./framework/framework.h"
#include "./utils/fRect.h"
#include "./utils/clock.h"



Game::Game() {
};


Game::~Game() {
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
                }
            }
        }


        // realRect.setX(realRect.x() + deltaTime);
        realRect.pivotRect(450, 450, deltaTime * 2);

        SDL_SetRenderDrawColor(Framework::display.renderer(), 0, 0, 0, 255);
        SDL_RenderClear(Framework::display.renderer());
        SDL_RenderTexture(Framework::display.renderer(), img, nullptr, &realRect.getSDLFRect());
        // SDL_RenderTextureRotated(Framework::display.renderer(), img, nullptr, &srcRect, angle, nullptr, SDL_FLIP_NONE);
        SDL_RenderPresent(Framework::display.renderer());
    }
};