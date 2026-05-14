#include <functional>
#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "../sdlUtils/font.h"
#include "./liveTextDisplay.h"



LiveTextDisplay::LiveTextDisplay(
    float x, float y, Font* font, const Color& color, const std::function<std::string()>& textGetter
) : m_textGetterCb{textGetter}, m_font{font}, m_color{color} {
    m_rect.setTopleft(x, y);

    if (!isValid()) {
        return;
    }

    updateSurface(m_textGetterCb());
};

LiveTextDisplay::LiveTextDisplay() {
};

LiveTextDisplay::LiveTextDisplay(const LiveTextDisplay& display) 
    : m_textGetterCb{display.m_textGetterCb}, 
    m_font{display.m_font}, 
    m_color{display.m_color}, 
    m_rect{display.m_rect},
    m_centerX{display.m_centerX},
    m_centerY{display.m_centerY},
    m_useCenter{display.m_useCenter} 
{   
    if (!isValid()) {
        return;
    }

    updateSurface(m_textGetterCb());
};

LiveTextDisplay::LiveTextDisplay(LiveTextDisplay&& display) 
    : m_textGetterCb{display.m_textGetterCb}, 
    m_font{display.m_font}, 
    m_color{display.m_color}, 
    m_rect{display.m_rect},
    m_centerX{display.m_centerX},
    m_centerY{display.m_centerY},
    m_useCenter{display.m_useCenter}
{
    if (!isValid()) {
        return;
    }

    updateSurface(m_textGetterCb());
};

LiveTextDisplay::~LiveTextDisplay() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroySurface(m_surface);
};

LiveTextDisplay& LiveTextDisplay::operator=(const LiveTextDisplay& display) {
    if (this == &display) {
        return *this;
    }

    m_textGetterCb = display.m_textGetterCb;

    m_rect = display.m_rect;
    m_centerX = display.m_centerX;
    m_centerY = display.m_centerY;
    m_useCenter = display.m_useCenter;
    
    m_font = display.m_font;
    m_color = display.m_color;

    clearStoredRenders();

    if (isValid()) {
        updateSurface(m_textGetterCb());
    }

    return *this;
};

LiveTextDisplay& LiveTextDisplay::operator=(LiveTextDisplay&& display) {
    if (this == &display) {
        return *this;
    }

    m_textGetterCb = display.m_textGetterCb;

    m_rect = display.m_rect;
    m_centerX = display.m_centerX;
    m_centerY = display.m_centerY;
    m_useCenter = display.m_useCenter;
    
    m_font = display.m_font;
    m_color = display.m_color;

    clearStoredRenders();

    if (isValid()) {
        updateSurface(m_textGetterCb());
    }

    return *this;
};


void LiveTextDisplay::update(SDL_Renderer* renderer) {
    if (!isValid()) {
        return;
    }

    std::string newValue{m_textGetterCb()};
    if (newValue != m_lastValue || !m_texture) {
        m_lastValue = newValue;
        updateRender(renderer);
    }

    SDL_RenderTexture(renderer, m_texture, nullptr, &m_rect.getSDLFRect());
};


void LiveTextDisplay::setTextGetter(const std::function<std::string()>& getter) {
    m_textGetterCb = getter;
    clearStoredRenders();

    if (isValid()) {
        updateSurface(m_textGetterCb());
    }
};


void LiveTextDisplay::setFont(Font* font) {
    m_font = font;
    clearStoredRenders();

    if (isValid()) {
        updateSurface(m_textGetterCb());
    }
};


void LiveTextDisplay::setTopleft(float x, float y) {
    m_rect.setTopleft(x, y);
};


void LiveTextDisplay::setCenter(float x, float y) {
    m_centerX = x;
    m_centerY = y;
    m_rect.setCenter(x, y);
    m_useCenter = true;
};


int LiveTextDisplay::width() const {
    if (!isValid()) {
        return 0;
    }

    return m_surface->w;
};


int LiveTextDisplay::height() const {
    if (!isValid()) {
        return 0;
    }

    return m_surface->h;
};


bool LiveTextDisplay::isValid() const {
    return (m_font && m_textGetterCb);
};


void LiveTextDisplay::updateRender(SDL_Renderer* renderer) {
    SDL_DestroyTexture(m_texture);
    updateSurface(m_lastValue);
    m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);
};


void LiveTextDisplay::updateSurface(const std::string& text) {
    SDL_DestroySurface(m_surface);
    m_surface = m_font->renderSurface(text.data(), m_color.getSDLColor());

    m_rect.setSize(static_cast<float>(m_surface->w), static_cast<float>(m_surface->h));
    if (m_useCenter) {
        m_rect.setTopleft(m_centerX - m_rect.width() / 2, m_centerY - m_rect.height() / 2);
    }
};


void LiveTextDisplay::clearStoredRenders() {
    SDL_DestroyTexture(m_texture);
    SDL_DestroySurface(m_surface);
    m_texture = nullptr;
    m_surface = nullptr;
    m_lastValue = "";
};