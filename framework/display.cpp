#include <stdexcept>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include "./display.h"



Display::Display(int canvasWidth, int canvasHeight, SDL_RendererLogicalPresentation rendererPresentationFlag) 
    : m_canvasWidth{canvasWidth}, m_canvasHeight{canvasHeight} {
    if (created) {
        SDL_Log("Display already initialized\n");
        throw std::runtime_error("Display already initialized\n");
    }

    bool initSucess {SDL_Init(SDL_INIT_VIDEO)};
    if (!initSucess) {
        SDL_Log("Unable to init video system: %s", SDL_GetError());
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }


    m_screen = SDL_CreateWindow("Breakout", 0, 0, SDL_WINDOW_FULLSCREEN);
    if (!m_screen) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }
    SDL_GetWindowSizeInPixels(m_screen, &m_screenWidth, &m_screenHeight);


    m_renderer = SDL_CreateRenderer(m_screen, nullptr);
    if (!m_renderer) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_Quit();
        throw std::runtime_error(SDL_GetError());
    }
    SDL_SetRenderLogicalPresentation(m_renderer, m_canvasWidth, m_canvasHeight, rendererPresentationFlag);

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