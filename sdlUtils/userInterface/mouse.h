#ifndef MY_SDL_MOUSE_H
#define MY_SDL_MOUSE_H


#include <string_view>
#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "../collision/fRect.h"


class Mouse {
    FRect m_rect{0.0f, 0.0f, 1.0f, 1.0f};
    SDL_Texture* m_img {nullptr};
    float m_imgXOffset {};
    float m_imgYOffset {};

    float m_screenWidth {};
    float m_screenHeight {};
    float m_canvasWidth {};
    float m_canvasHeight {};

    bool m_clampToCanvas {false};
    bool m_mouseHidden {false};

    float m_xPos {};
    float m_yPos {};
    float m_sensitivity {1.0f};

    std::string m_saveFilePath{};


public:
    Mouse(
        float screenWidth, 
        float screenHeight, 
        float canvasWidth, 
        float canvasHeight,
        SDL_Window* window,
        SDL_Texture* img, 
        float imgXOffset=0.0f,
        float imgYOffset=0.0f,
        std::string_view saveFilePath=""
    );

    Mouse(const Mouse&) = delete;
    Mouse(Mouse&&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    Mouse& operator=(Mouse&&) = delete;
    ~Mouse() = default;

    // Method to update the current position of the mouse.
    // Expects the relative mouse coords from the SDL_MOUSE_MOTION_EVENT
    void update(float relX, float relY);

    // Draw the mouse img to screen if there is one.
    void draw(SDL_Renderer* renderer);

    // Set if the mouse should be constrained to the canvas. Default is not clamped.
    void clampMouseToCanvas(bool clamp);

    // Query if the mouse is currently constrained to the canvas.
    bool mouseClamped() const;

    // Get the current mouse position.
    SDL_FPoint getPos() const;

    // Get the current mouse rect. If there is no mouse image the rect will have a width and height of 1,
    // and the rect's topleft will be the mouse position.
    const FRect& getRect() const;

    // Set if the mouse image should be drawn. Default is not hidden.
    void setMouseHidden(bool hide);

    // Query if the mouse image is currently hidden.
    bool mouseHidden() const;

    // Get the current sensitivity setting.
    float getSensitivity() const;

    // Set the mouse sensitivity. This value will be clamped to be no less than 0.1.
    void setSensitivity(float newSensitivity);

    // Save the current sensitivity setting.
    void saveSensitivitySetting() const;



private:
    void remapCoords(float* x, float* y);

    void clampToCanvas();

    void roundCoords();

    void loadSensitivitySetting();
};



#endif