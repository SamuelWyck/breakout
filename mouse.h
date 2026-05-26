#ifndef MY_SDL_MOUSE_H
#define MY_SDL_MOUSE_H


#include <utility>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>
#include "./sdlUtils/math.h"
#include "./sdlUtils/collision/fRect.h"
#include <iostream>


class Mouse {
    FRect m_rect{};
    SDL_Texture* m_img {nullptr};
    float m_imgXOffset {};
    float m_imgYOffset {};

    float m_screenWidth {};
    float m_screenHeight {};
    float m_canvasWidth {};
    float m_canvasHeight {};

    float m_xPos {};
    float m_yPos {};
    float m_sensitivity {1.0f};


public:
    Mouse(
        SDL_Texture* img, 
        float screenWidth, 
        float screenHeight, 
        float canvasWidth, 
        float canvasHeight,
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

        m_rect.setSize(static_cast<float>(m_img->w), static_cast<float>(m_img->h));
        m_rect.setTopleft(m_xPos - m_imgXOffset, m_yPos - m_imgYOffset);
    };

    Mouse(const Mouse&) = delete;
    Mouse(Mouse&&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    Mouse& operator=(Mouse&&) = delete;
    ~Mouse() = default;


    void update(float relX, float relY, FRect* boundingRect=nullptr) {
        remapCoords(&relX, &relY);
        m_xPos += (relX * m_sensitivity);
        m_yPos += (relY * m_sensitivity);

        if (boundingRect != nullptr) {
            clampToBoundingRect(boundingRect);
        }

        m_rect.setTopleft(m_xPos - m_imgXOffset, m_yPos - m_imgYOffset);
    };


    void draw(SDL_Renderer* renderer) {
        SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
    };


    std::pair<float, float> getPos() const {
        return std::pair<float, float>{m_xPos, m_yPos};
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


    void clampToBoundingRect(FRect* boundingRect) {
        if (m_xPos < boundingRect->x()) {
            m_xPos = boundingRect->x();

        } else if (m_xPos > boundingRect->right()) {
            m_xPos = boundingRect->right();
        }

        if (m_yPos < boundingRect->y()) {
            m_yPos = boundingRect->y();

        } else if (m_yPos > boundingRect->bottom()) {
            m_yPos = boundingRect->bottom();
        }
    };
};



#endif