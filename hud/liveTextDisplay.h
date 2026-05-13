#ifndef LIVE_TEXT_DISPLAY_H
#define LIVE_TEXT_DISPLAY_H


#include <functional>
#include <string>
#include <SDL3/SDL_render.h>
#include "../sdlUtils/fRect.h"
#include "../sdlUtils/color.h"
#include "../sdlUtils/font.h"


class LiveTextDisplay {
    std::function<std::string()> m_textGetterCb{nullptr};
    std::string m_lastValue{};
    bool m_initialRenderMade {false};

    Font* m_font {nullptr};
    Color m_color{};

    FRect m_rect{};
    SDL_Texture* m_texture {nullptr};

    float m_centerX {};
    float m_centerY {};
    bool m_useCenter {false};


public:
    LiveTextDisplay(float x, float y, Font* font, const Color& color, const std::function<std::string()> textGetter)
        : m_textGetterCb{textGetter}, m_font{font}, m_color{color} 
    {
        m_rect.setTopleft(x, y);
    };

    LiveTextDisplay() {
    };

    LiveTextDisplay(const LiveTextDisplay& display) 
        : m_textGetterCb{display.m_textGetterCb}, 
        m_font{display.m_font}, 
        m_color{display.m_color}, 
        m_rect{display.m_rect},
        m_centerX{display.m_centerX},
        m_centerY{display.m_centerY},
        m_useCenter{display.m_useCenter} {
    };

    LiveTextDisplay(LiveTextDisplay&& display) 
        : m_textGetterCb{display.m_textGetterCb}, 
        m_font{display.m_font}, 
        m_color{display.m_color}, 
        m_rect{display.m_rect},
        m_centerX{display.m_centerX},
        m_centerY{display.m_centerY},
        m_useCenter{display.m_useCenter}
    {
        m_texture = display.m_texture;
        display.m_texture = nullptr;
        m_initialRenderMade = true;
    };

    ~LiveTextDisplay() {
        SDL_DestroyTexture(m_texture);
    };

    LiveTextDisplay& operator=(const LiveTextDisplay& display) {
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

        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
        m_initialRenderMade = false;
        m_lastValue = "";

        return *this;
    };

    LiveTextDisplay& operator=(LiveTextDisplay&& display) {
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

        SDL_DestroyTexture(m_texture);
        m_texture = display.m_texture;
        display.m_texture = nullptr;

        m_lastValue = display.m_lastValue;

        return *this;
    };


    void update(SDL_Renderer* renderer) {
        if (!m_textGetterCb || !m_font) {
            return;
        }

        std::string newValue{m_textGetterCb()};
        if (newValue != m_lastValue || !m_initialRenderMade) {
            updateRender(newValue, renderer);
            m_initialRenderMade = true;
        }

        SDL_RenderTexture(renderer, m_texture, nullptr, &m_rect.getSDLFRect());
    };


    void setTextGetter(const std::function<std::string()>& getter) {
        m_textGetterCb = getter;
        m_initialRenderMade = false;
    };


    void setTopleft(float x, float y) {
        m_rect.setTopleft(x, y);
    };


    void setCenter(float x, float y) {
        m_centerX = x;
        m_centerY = y;
        m_useCenter = true;
        m_rect.setCenter(x, y);
    };


private:
    void updateRender(const std::string& value, SDL_Renderer* renderer) {
        SDL_DestroyTexture(m_texture);
        m_texture = m_font->renderTexture(renderer, value.data(), m_color.getSDLColor());

        m_rect.setSize(static_cast<float>(m_texture->w), static_cast<float>(m_texture->h));
        if (m_useCenter) {
            m_rect.setCenter(m_centerX, m_centerY);
        }
    };
};



#endif 