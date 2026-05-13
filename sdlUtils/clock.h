#ifndef CLOCK_H
#define CLOCK_H


#include <cstdint>


class Clock {
    std::uint64_t m_lastTime {};
    std::uint64_t m_currentTime {};


public:
    // SDL must be initialized first 
    Clock();

    // Wrapper for SDL_GetTicks
    std::uint64_t getTicks() const;

    // Get the number of ms since the last time this function was called
    std::uint64_t getDeltaTime();

    // Delta time normalized to 1 = 1/60 of a second aka 1 frame at 60 frames per sec
    double getNormalizedDeltaTime();
};



#endif