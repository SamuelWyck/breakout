#ifndef LIVE_TEXT_DISPLAY_H
#define LIVE_TEXT_DISPLAY_H


#include <functional>
#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "../sdlUtils/collision/fRect.h"
#include "../sdlUtils/color.h"
#include "../sdlUtils/font.h"


class LiveTextDisplay {
    std::function<std::string()> m_textGetterCb{nullptr};
    std::string m_lastValue{};

    Font* m_font {nullptr};
    Color m_color{};

    SDL_Surface* m_surface {nullptr};
    SDL_Texture* m_texture {nullptr};
    
    FRect m_rect{};
    float m_centerX {};
    float m_centerY {};
    bool m_useCenter {false};


public:
    LiveTextDisplay(float x, float y, Font* font, const Color& color, const std::function<std::string()>& textGetter);
    LiveTextDisplay();
    LiveTextDisplay(const LiveTextDisplay& display);
    LiveTextDisplay(LiveTextDisplay&& display);
    ~LiveTextDisplay();
    LiveTextDisplay& operator=(const LiveTextDisplay& display);
    LiveTextDisplay& operator=(LiveTextDisplay&& display);


    void update(SDL_Renderer* renderer);

    void setTextGetter(const std::function<std::string()>& getter);

    void setFont(Font* font);

    void setTopleft(float x, float y);

    void setCenter(float x, float y);

    int width() const;

    int height() const;

    bool isValid() const;


private:
    void updateRender(SDL_Renderer* renderer);

    void updateSurface(const std::string& text);

    void clearStoredRenders();
};



#endif 