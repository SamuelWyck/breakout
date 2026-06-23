#include <string_view>
#include <string>
#include <stdexcept>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include "./display.h"



Display::Display(
    std::string_view title,
    int screenWidth,
    int screenHeight,
    int canvasWidth, 
    int canvasHeight, 
    int vsyncRate,
    SDL_WindowFlags windowFlags,
    SDL_RendererLogicalPresentation rendererPresentationFlag
) 
    : m_screenWidth{screenWidth}, 
    m_screenHeight{screenHeight}, 
    m_canvasWidth{canvasWidth}, 
    m_canvasHeight{canvasHeight},
    m_vsyncRate{vsyncRate}
{
    if (Display::created) {
        SDL_Log("Display already initialized\n");
        throw std::runtime_error("Display already initialized\n");
    }

    bool initSucess {SDL_Init(SDL_INIT_VIDEO)};
    if (!initSucess) {
        std::string error{SDL_GetError()};
        SDL_Quit();
        throw std::runtime_error(error);
    }


    m_screen = SDL_CreateWindow(title.data(), m_screenWidth, m_screenHeight, windowFlags);
    if (!m_screen) {
        std::string error{SDL_GetError()};
        SDL_Quit();
        throw std::runtime_error(error);
    }
    SDL_GetWindowSizeInPixels(m_screen, &m_screenWidth, &m_screenHeight);


    m_renderer = SDL_CreateRenderer(m_screen, nullptr);
    if (!m_renderer) {
        std::string error{SDL_GetError()};
        SDL_Quit();
        throw std::runtime_error(error);
    }
    SDL_SetRenderLogicalPresentation(m_renderer, m_canvasWidth, m_canvasHeight, rendererPresentationFlag);
    if (!SDL_SetRenderVSync(m_renderer, m_vsyncRate)) {
        m_vsyncRate = 0;
    }


    Display::created = true;
};


Display::~Display() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_screen);
    SDL_Quit();
    Display::created = false;
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


int Display::getVsync() const {
    return m_vsyncRate;
};

bool Display::setVsync(int vsyncRate) {
    m_vsyncRate = vsyncRate;
    bool success {SDL_SetRenderVSync(m_renderer, vsyncRate)};
    if (!success) {
        m_vsyncRate = 0;
    }
    return success;
};


SDL_Window* Display::window() {
    return m_screen;
};

int Display::screenWidth() {
    return m_screenWidth;
};

int Display::screenHeight() {
    return m_screenHeight;
};