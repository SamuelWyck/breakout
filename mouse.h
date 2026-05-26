#ifndef MY_SDL_MOUSE_H
#define MY_SDL_MOUSE_H


#include <cmath>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "./sdlUtils/math.h"
#include "./sdlUtils/collision/fRect.h"


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


public:
    Mouse(
        float screenWidth, 
        float screenHeight, 
        float canvasWidth, 
        float canvasHeight,
        SDL_Texture* img=nullptr, 
        float imgXOffset=0.0f,
        float imgyOffset=0.0f
    )
        : m_img{img}, 
        m_imgXOffset{imgXOffset},
        m_imgYOffset{imgyOffset},
        m_screenWidth{screenWidth}, 
        m_screenHeight{screenHeight}, 
        m_canvasWidth{canvasWidth}, 
        m_canvasHeight{canvasHeight}
    {
        SDL_GetGlobalMouseState(&m_xPos, &m_yPos);
        remapCoords(&m_xPos, &m_yPos);
        roundCoords();

        if (m_img != nullptr) {
            m_rect.setSize(static_cast<float>(m_img->w), static_cast<float>(m_img->h));
        }
        m_rect.setTopleft(m_xPos - m_imgXOffset, m_yPos - m_imgYOffset);
    };

    Mouse(const Mouse&) = delete;
    Mouse(Mouse&&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    Mouse& operator=(Mouse&&) = delete;
    ~Mouse() = default;


    void update(float relX, float relY) {
        remapCoords(&relX, &relY);
        m_xPos += (relX * m_sensitivity);
        m_yPos += (relY * m_sensitivity);

        if (m_clampToCanvas) {
            clampToCanvas();
        }
        roundCoords();

        m_rect.setTopleft(m_xPos - m_imgXOffset, m_yPos - m_imgYOffset);
    };


    void draw(SDL_Renderer* renderer) {
        if (m_img == nullptr || m_mouseHidden) {
            return;
        }
        SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
    };


    void clampMouseToCanvas(bool clamp) {
        m_clampToCanvas = clamp;
    };


    bool mouseClamped() const {
        return m_clampToCanvas;
    };


    SDL_FPoint getPos() const {
        return SDL_FPoint{m_xPos, m_yPos};
    };


    const FRect& getRect() const {
        return m_rect;
    };


    void setMouseHidden(bool hide) {
        m_mouseHidden = hide;
    };


    bool mouseHidden() const {
        return m_mouseHidden;
    };


    float getSensitivity() const {
        return m_sensitivity;
    };


    void setSensitivity(float newSensitivity) {
        if (newSensitivity < .1f) {
            newSensitivity = .1f;
        }

        m_sensitivity = newSensitivity;
    };


private:
    void remapCoords(float* x, float* y) {
        *x = Math::remap(0.0f, m_screenWidth, 0.0f, m_canvasWidth, *x);
        *y = Math::remap(0.0f, m_screenHeight, 0.0f, m_canvasHeight, *y);
    };


    void clampToCanvas() {
        if (m_xPos < 0.0f) {
            m_xPos = 0.0f;

        } else if (m_xPos > m_canvasWidth) {
            m_xPos = m_canvasWidth;
        }

        if (m_yPos < 0.0f) {
            m_yPos = 0.0f;

        } else if (m_yPos > m_canvasHeight) {
            m_yPos = m_canvasHeight;
        }
    };


    void roundCoords() {
        m_xPos = roundf(m_xPos * 1000.0f) / 1000.0f;
        m_yPos = roundf(m_yPos * 1000.0f) / 1000.0f;
    };
};



#endif