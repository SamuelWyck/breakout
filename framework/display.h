#ifndef DISPLAY_H
#define DISPLAY_H


#include <SDL3/SDL_render.h>


class Display {
    SDL_Window* m_screen {nullptr};
    SDL_Renderer* m_renderer {nullptr};

    int m_screenWidth {};
    int m_screenHeight {};
    int m_canvasWidth {};
    int m_canvasHeight {};

    inline static bool created {false};


public:
    Display(
        int canvasWidth, 
        int canvasHeight, 
        SDL_RendererLogicalPresentation rendererPresentationFlag=SDL_LOGICAL_PRESENTATION_DISABLED
    );

    ~Display();


    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&&) = delete;


    SDL_Renderer* renderer();;

    int canvasWidth();

    int canvasHeight();
};



#endif