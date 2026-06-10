#ifndef DISPLAY_H
#define DISPLAY_H


#include <string_view>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>


class Display {
    SDL_Window* m_screen {nullptr};
    SDL_Renderer* m_renderer {nullptr};

    int m_screenWidth {};
    int m_screenHeight {};
    int m_canvasWidth {};
    int m_canvasHeight {};

    SDL_Texture* m_renderCopy {nullptr};

    inline static bool created {false};


public:
    Display(
        std::string_view title, 
        int screenWidth, 
        int screenHeight,
        int canvasWidth, 
        int canvasHeight,
        SDL_WindowFlags windowFlags=0,
        SDL_RendererLogicalPresentation rendererPresentationFlag=SDL_LOGICAL_PRESENTATION_DISABLED
    );

    ~Display();


    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(Display&&) = delete;


    SDL_Renderer* renderer();

    SDL_Window* window();

    void targetTexture();

    SDL_Texture* targetWindow();

    int canvasWidth();

    int canvasHeight();


    SDL_Window* screen();

    int screenWidth();

    int screenHeight();
};



#endif