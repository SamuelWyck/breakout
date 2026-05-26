#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H


#include <string>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "../sdlUtils/font.h"
#include "../sdlUtils/color.h"
#include "../sdlUtils/collision/fRect.h"


class TextDisplay {
    SDL_Texture* m_img {nullptr};
    SDL_Surface* m_surface {nullptr};

    FRect m_rect{};
    bool m_useCenter {false};
    float m_centerX {};
    float m_centerY {};

    Font* m_font {nullptr};
    Color m_color{};

    std::string m_text{};


public:
    TextDisplay(float x, float y, const std::string& text, Font* font, const Color& color);
    TextDisplay();
    TextDisplay(const TextDisplay&) = delete;
    TextDisplay(TextDisplay&&) = delete;
    TextDisplay& operator=(const TextDisplay&) = delete;
    TextDisplay& operator=(TextDisplay&&) = delete;
    ~TextDisplay();


    bool isValid() const;

    void update(SDL_Renderer* renderer);

    void setFont(Font* font);

    void setCenter(float x, float y);

    void setTopleft(float x, float y);

    int width() const;

    int height() const;

    void changeText(const std::string& text);


private:
    void updateSurface(const std::string& text);

    void updateTexture(SDL_Renderer* renderer);

    void clearStoredRenders();
};



#endif