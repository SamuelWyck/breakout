#ifndef SLIDER_ELEMENT_H
#define SLIDER_ELEMENT_H


#include <utility>
#include <functional>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "./iMenuElement.h"
#include "../../collision/fRect.h"


class Slider : public IMenuElement {
    FRect m_barRect{};
    FRect m_slideRect{};
    float m_slidePadding {};

    SDL_Texture* m_barImg {nullptr};
    SDL_Texture* m_slideImg {nullptr};
    SDL_Color m_barColor{};
    SDL_Color m_slideColor{};

    std::function<void(float)> m_slideCb{nullptr};

    bool m_mousePressed {false};


public:
    Slider(
        float x, float y, 
        const std::function<void(float)>& slideCb, 
        SDL_Texture* barImg, 
        SDL_Texture* slideImg
    );

    Slider(
        float x, float y, 
        const std::function<void(float)>& slideCb, 
        const SDL_Color& barColor,
        const SDL_Color& slideColor,
        float barWidth,
        float barHeight
    );

    Slider(const Slider&) = default;
    Slider(Slider&&) = default;
    Slider& operator=(const Slider&) = default;
    Slider& operator=(Slider&&) = default;
    ~Slider() = default;


    void update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mouseJustPressed, bool mouseJustReleased);

    
    float value() const;


    void setValue(float val);



    // Methods for menu element interface

    float width() const override;
    float height() const override;

    std::pair<float, float> topleft() const override;
    void setTopleft(float x, float y) override;

    std::pair<float, float> center() const override;
    void setCenter(float x, float y) override;

    void setX(float x) override;
    void setY(float y) override;

    void update(SDL_Renderer*) override;
    
    
    
private:
    void draw(SDL_Renderer* renderer);

    void setSliderPos(float x);

    void clampSlidePos();

    float barPaddedLeft() const;

    float barPaddedRight() const;

    float roundValue(float val) const;
};



#endif