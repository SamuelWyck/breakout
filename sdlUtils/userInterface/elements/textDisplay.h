#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H


#include <string>
#include <utility>
#include <string_view>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include "./iMenuElement.h"
#include "../../font.h"
#include "../../color.h"
#include "../../collision/fRect.h"


class TextDisplay : public IMenuElement {
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
    TextDisplay(float x, float y, std::string_view text, Font* font, const Color& color);
    TextDisplay();
    TextDisplay(const TextDisplay&);
    TextDisplay(TextDisplay&&) = delete;
    TextDisplay& operator=(const TextDisplay&) = delete;
    TextDisplay& operator=(TextDisplay&&) = delete;
    ~TextDisplay();


    bool isValid() const;

    void setFont(Font* font);

    void changeText(std::string_view text);

    void update(SDL_Renderer* renderer) override;

    std::pair<float, float> center() const override;
    void setCenter(float x, float y) override;

    std::pair<float, float> topleft() const override;
    void setTopleft(float x, float y) override;

    float width() const override;
    float height() const override;

    void setX(float x) override;
    void setY(float y) override; 



private:
    void updateSurface(std::string_view text);

    void updateTexture(SDL_Renderer* renderer);

    void clearStoredRenders();
};



#endif