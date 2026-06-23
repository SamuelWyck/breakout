#include <string>
#include <string_view>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "../../font.h"
#include "../../color.h"
#include "../../collision/fRect.h"
#include "./textDisplay.h"



TextDisplay::TextDisplay(float x, float y, std::string_view text, Font* font, const Color& color, int wrapLength) 
    : m_font{font}, m_color{color}, m_wrapLength{wrapLength} 
{
    m_rect.setTopleft(x, y);
    if (isValid()) {
        updateSurface(text);
    }
};

TextDisplay::TextDisplay() {
};

TextDisplay::TextDisplay(const TextDisplay& other)
    : m_rect{other.m_rect},
    m_useCenter{other.m_useCenter},
    m_centerX{other.m_centerX},
    m_centerY{other.m_centerY},
    m_font{other.m_font},
    m_color{other.m_color}
{
    if (isValid()) {
        updateSurface(other.m_text);
    }
};

TextDisplay::~TextDisplay() {
    SDL_DestroyTexture(m_img);
    SDL_DestroySurface(m_surface);
};


bool TextDisplay::isValid() const {
    return m_font != nullptr;
};


void TextDisplay::update(SDL_Renderer* renderer) {
    if (!isValid()) {
        return;
    }

    if (m_img == nullptr) {
        updateTexture(renderer);
    }
    
    SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
};


void TextDisplay::setFont(Font* font) {
    m_font = font;
    
    clearStoredRenders();
    
    if (isValid()) {
        updateSurface(m_text);
    }
};


void TextDisplay::changeText(std::string_view text) {
    clearStoredRenders();
    updateSurface(text);
};


void TextDisplay::setWrapLength(int wrapLength) {
    m_wrapLength = wrapLength;
    if (m_wrapLength < -1) {
        m_wrapLength = -1; // SDL uses -1 as a signal to disable wrapping
    }

    changeText(m_text);
};


int TextDisplay::wrapLength() const {
    return m_wrapLength;
};


std::pair<float, float> TextDisplay::center() const {
    return m_rect.center();
};


void TextDisplay::setCenter(float x, float y) {
    m_rect.setCenter(x, y);
    m_centerX = x;
    m_centerY = y;
    m_useCenter = true;
};


std::pair<float, float> TextDisplay::topleft() const {
    return m_rect.topleft();
};


void TextDisplay::setTopleft(float x, float y) {
    m_rect.setTopleft(x, y);
    m_useCenter = false;
};


float TextDisplay::width() const {
    if (!isValid()) {
        return 0;
    }
    return m_rect.width();
};


float TextDisplay::height() const {
    if (!isValid()) {
        return 0;
    }
    return m_rect.height();
};


void TextDisplay::setX(float x) {
    m_useCenter = false;
    m_rect.setX(x);
};


void TextDisplay::setY(float y) {
    m_useCenter = false;
    m_rect.setY(y);
};



void TextDisplay::updateSurface(std::string_view text) {
    m_text = text;
    SDL_DestroySurface(m_surface);
    m_surface = m_font->renderSurface(text, m_color.getSDLColor(), m_wrapLength);

    m_rect.setSize(static_cast<float>(m_surface->w), static_cast<float>(m_surface->h));
    if (m_useCenter) {
        m_rect.setCenter(m_centerX, m_centerY);
    }
};


void TextDisplay::updateTexture(SDL_Renderer* renderer) {
    if (m_surface == nullptr) {
        return;
    }

    SDL_DestroyTexture(m_img);
    m_img = SDL_CreateTextureFromSurface(renderer, m_surface);
};


void TextDisplay::clearStoredRenders() {
    SDL_DestroySurface(m_surface);
    SDL_DestroyTexture(m_img);
    m_img = nullptr;
    m_surface = nullptr;
};