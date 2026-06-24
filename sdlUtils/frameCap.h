#ifndef MY_FRAMECAP_H
#define MY_FRAMECAP_H


#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_stdinc.h>


class FrameCap {
    Uint64 m_lastTime {SDL_GetTicks()};



public:
    FrameCap();


    void start();


    void capFrames(int framerate);
};



#endif