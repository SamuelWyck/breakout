#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_stdinc.h>
#include "./frameCap.h"



FrameCap::FrameCap() {
};


void FrameCap::start() {
    m_lastTime = SDL_GetTicks();
};


void FrameCap::capFrames(int framerate) {
    if (framerate <= 0) {
        framerate = 1;
    }

    constexpr Uint64 oneSecond {1000};

    Uint64 idealRate {oneSecond / framerate};
    Uint64 deltaTime {SDL_GetTicks() - m_lastTime};

    if (deltaTime < idealRate) {
        SDL_Delay(idealRate - deltaTime);
    }

    m_lastTime = SDL_GetTicks();
};