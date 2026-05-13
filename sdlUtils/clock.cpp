#include <cstdint>
#include <SDL3/SDL_timer.h>
#include "./clock.h"



// SDL must be initialized first 
Clock::Clock() : m_lastTime{SDL_GetTicks()}, m_currentTime{SDL_GetTicks()} {
};

// Wrapper for SDL_GetTicks
std::uint64_t Clock::getTicks() const {
    return SDL_GetTicks();
};

// Get the number of ms since the last time this function was called
std::uint64_t Clock::getDeltaTime() {
    m_currentTime = SDL_GetTicks();
    std::uint64_t deltaTime {m_currentTime - m_lastTime};
    m_lastTime = m_currentTime;
    return deltaTime;
};

// Delta time normalized to 1 = 1/60 of a second 
double Clock::getNormalizedDeltaTime() {
    std::uint64_t deltaTime {getDeltaTime()};
    double deltaTimeSecs {deltaTime / 1000.0};
    deltaTimeSecs *= 60;
    return deltaTimeSecs;
};