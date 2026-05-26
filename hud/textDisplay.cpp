#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "../sdlUtils/font.h"
#include "../sdlUtils/color.h"
#include "../sdlUtils/collision/fRect.h"
#include "./textDisplay.h"



TextDisplay::TextDisplay(float x, float y, const std::string& text, Font* font, const Color& color) 
    : m_font{font}, m_color{color} 
{
    m_rect.setTopleft(x, y);
    if (isValid()) {
        updateSurface(text);
    }
};

TextDisplay::TextDisplay() {
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


void TextDisplay::setCenter(float x, float y) {
    m_rect.setCenter(x, y);
    m_centerX = x;
    m_centerY = y;
    m_useCenter = true;
};


void TextDisplay::setTopleft(float x, float y) {
    m_rect.setTopleft(x, y);
    m_useCenter = false;
};


int TextDisplay::width() const {
    if (!isValid()) {
        return 0;
    }
    return m_surface->w;
};


int TextDisplay::height() const {
    if (!isValid()) {
        return 0;
    }
    return m_surface->h;
};


void TextDisplay::changeText(const std::string& text) {
    clearStoredRenders();
    updateSurface(text);
};


void TextDisplay::updateSurface(const std::string& text) {
    m_text = text;
    SDL_DestroySurface(m_surface);
    m_surface = m_font->renderSurface(text, m_color.getSDLColor());

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