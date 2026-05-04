#include <SDL3/SDL.h>
#include <iostream>
#include <exception>
#include "./display.h"



Display::Display(int canvasWidth, int canvasHeight) 
    : m_canvasWidth{canvasWidth}, m_canvasHeight{canvasHeight} {
    if (created) {
        SDL_Log("Display already initialized\n");
        throw std::runtime_error("Display already initialized\n");
    }

    bool initSucess {SDL_Init(SDL_INIT_VIDEO)};
    if (!initSucess) {
        SDL_Log("Unable to init video system: %s", SDL_GetError());
        std::cerr << "Unable to init video system: " << SDL_GetError() << "\n";
        SDL_Quit();
        throw -1;
    }


    m_screen = SDL_CreateWindow("Breakout", 0, 0, SDL_WINDOW_FULLSCREEN);
    if (!m_screen) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        std::cerr << "Unable to create window: " << SDL_GetError() << "\n";
        SDL_Quit();
        throw -1;
    }
    SDL_GetWindowSizeInPixels(m_screen, &m_screenWidth, &m_screenHeight);


    m_renderer = SDL_CreateRenderer(m_screen, nullptr);
    if (!m_renderer) {
        std::cerr << "Unable to create renderer: " << SDL_GetError() << "\n";
        SDL_Quit();
        throw -1;
    }
    SDL_SetRenderLogicalPresentation(m_renderer, m_canvasWidth, m_canvasHeight, SDL_LOGICAL_PRESENTATION_STRETCH);

    created = true;
};


Display::~Display() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_screen);
};


SDL_Renderer* Display::renderer() {
    return m_renderer;
};

int Display::canvasWidth() {
    return m_canvasWidth;
};

int Display::canvasHeight() {
    return m_canvasHeight;
};