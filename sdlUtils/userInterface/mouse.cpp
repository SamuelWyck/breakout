#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string_view>
#include <string>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "./mouse.h"
#include "../math.h"
#include "../collision/fRect.h"



Mouse::Mouse(
    float screenWidth, 
    float screenHeight, 
    float canvasWidth, 
    float canvasHeight,
    SDL_Window* window,
    SDL_Texture* img, 
    float imgXOffset,
    float imgYOffset,
    std::string_view saveFilePath
)
    : m_img{img}, 
    m_imgXOffset{imgXOffset},
    m_imgYOffset{imgYOffset},
    m_screenWidth{screenWidth}, 
    m_screenHeight{screenHeight}, 
    m_canvasWidth{canvasWidth}, 
    m_canvasHeight{canvasHeight},
    m_saveFilePath{saveFilePath}
{   
    if (img == nullptr) {
        throw std::runtime_error("Mouse img cannot be nullptr.\n");
    }

    SDL_GetGlobalMouseState(&m_xPos, &m_yPos);
    remapCoords(&m_xPos, &m_yPos);
    roundCoords();

    m_rect.setSize(static_cast<float>(m_img->w), static_cast<float>(m_img->h));
    m_rect.setTopleft(m_xPos - m_imgXOffset, m_yPos - m_imgYOffset);

    loadSensitivitySetting();

    SDL_SetWindowRelativeMouseMode(window, true);
};



void Mouse::update(float relX, float relY) {
    remapCoords(&relX, &relY);
    m_xPos += (relX * m_sensitivity);
    m_yPos += (relY * m_sensitivity);

    if (m_clampToCanvas) {
        clampToCanvas();
    }
    roundCoords();

    m_rect.setTopleft(m_xPos - m_imgXOffset, m_yPos - m_imgYOffset);
};


void Mouse::draw(SDL_Renderer* renderer) {
    if (m_mouseHidden) {
        return;
    }
    SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
};


void Mouse::clampMouseToCanvas(bool clamp) {
    m_clampToCanvas = clamp;
};


bool Mouse::mouseClamped() const {
    return m_clampToCanvas;
};


SDL_FPoint Mouse::getPos() const {
    return SDL_FPoint{m_xPos, m_yPos};
};


const FRect& Mouse::getRect() const {
    return m_rect;
};


void Mouse::setMouseHidden(bool hide) {
    m_mouseHidden = hide;
};


bool Mouse::mouseHidden() const {
    return m_mouseHidden;
};


float Mouse::getSensitivity() const {
    return m_sensitivity;
};


void Mouse::setSensitivity(float newSensitivity) {
    if (newSensitivity < .1f) {
        newSensitivity = .1f;
    }

    m_sensitivity = newSensitivity;
};


void Mouse::saveSensitivitySetting() const {
    if (m_saveFilePath == "") {
        return;
    }

    try {
        std::ofstream file{m_saveFilePath};
        file << m_sensitivity;
        file.close();

    } catch (...) {
        return;
    }
};


void Mouse::remapCoords(float* x, float* y) {
    *x = Math::remap(0.0f, m_screenWidth, 0.0f, m_canvasWidth, *x);
    *y = Math::remap(0.0f, m_screenHeight, 0.0f, m_canvasHeight, *y);
};


void Mouse::clampToCanvas() {
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


void Mouse::roundCoords() {
    m_xPos = roundf(m_xPos * 1000.0f) / 1000.0f;
    m_yPos = roundf(m_yPos * 1000.0f) / 1000.0f;
};


void Mouse::loadSensitivitySetting() {
    if (m_saveFilePath == "") {
        return;
    }

    std::ifstream file{m_saveFilePath};
    if (file.fail()) {
        file.close();
        return;
    }

    try {
        std::string data{};
        file >> data;
        m_sensitivity = std::stof(data);

    } catch (...) {
        file.close();
        return;
    }
};