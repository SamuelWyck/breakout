#include <cmath>
#include <utility>
#include <functional>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include "../../math.h"
#include "./slider.h"



Slider::Slider(float x, float y, const std::function<void(float)>& slideCb, SDL_Texture* barImg, SDL_Texture* slideImg) 
    : m_barImg{barImg}, m_slideImg{slideImg}, m_slideCb{slideCb} 
{
    m_barRect.setTopleft(x, y);
    m_barRect.setSize(static_cast<float>(m_barImg->w), static_cast<float>(m_barImg->h));

    float sliderSize {static_cast<float>(m_slideImg->w)};
    m_slidePadding = m_barRect.height() * .1f;

    m_slideRect.setTopleft(x + m_slidePadding, y + m_slidePadding);
    m_slideRect.setSize(sliderSize, sliderSize);
};

Slider::Slider(
    float x, float y, 
    const std::function<void(float)>& slideCb, 
    const SDL_Color& barColor,
    const SDL_Color& slideColor,
    float barWidth,
    float barHeight
) 
    : m_barColor{barColor},
    m_slideColor{slideColor},
    m_slideCb{slideCb} 
{
    m_barRect.setTopleft(x, y);
    m_barRect.setSize(barWidth, barHeight);

    float sliderSize {m_barRect.height() * .8f};
    m_slidePadding = m_barRect.height() * .1f;
    
    m_slideRect.setTopleft(x + m_slidePadding, y + m_slidePadding);
    m_slideRect.setSize(sliderSize, sliderSize);
};


void Slider::update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mouseJustPressed, bool mouseJustReleased) {
    bool hovering {m_barRect.pointInRect(&mousePos)};
    auto [mouseX, mouseY] {mousePos};

    if (hovering && mouseJustPressed) {
        m_mousePressed = true;

        float currentVal {value()};
        setSliderPos(mouseX);
        if (value() != currentVal) {
            m_slideCb(value());
        }

    } else if (mouseJustReleased) {
        m_mousePressed = false;

    } else if (m_mousePressed) {
        float currentVal {value()};
        setSliderPos(mouseX);
        if (value() != currentVal) {
            m_slideCb(value());
        }
    }

    draw(renderer);
};


void Slider::draw(SDL_Renderer* renderer) {
    if (!m_barImg || !m_slideImg) {
        SDL_SetRenderDrawColor(renderer, m_barColor.r, m_barColor.g, m_barColor.b, m_barColor.a);
        SDL_RenderFillRect(renderer, &m_barRect.getSDLFRect());
        SDL_SetRenderDrawColor(renderer, m_slideColor.r, m_slideColor.g, m_slideColor.b, m_slideColor.a);
        SDL_RenderFillRect(renderer, &m_slideRect.getSDLFRect());

    } else {
        SDL_RenderTexture(renderer, m_barImg, nullptr, &m_barRect.getSDLFRect());
        SDL_RenderTexture(renderer, m_slideImg, nullptr, &m_slideRect.getSDLFRect());
    }
};


float Slider::value() const {
    float srcMin {barPaddedLeft() + (m_slideRect.width() / 2.0f)};
    float srcMax {barPaddedRight() - (m_slideRect.width() / 2.0f)};
    
    float val {Math::remap(srcMin, srcMax, 0.0f, 1.0f, m_slideRect.centerX())};
    return roundValue(val);
};


void Slider::setValue(float val) {
    if (val < 0.0f) {
        val = 0.0f;
    } else if (val > 1.0f) {
        val = 1.0f;
    }
    val = roundValue(val);
    
    float dstMin {barPaddedLeft() + (m_slideRect.width() / 2.0f)};
    float dstMax {barPaddedRight() - (m_slideRect.width() / 2.0f)};
    float xPos {val * (dstMax - dstMin) + dstMin};
    setSliderPos(xPos);
};



// Methods for menu element interface

float Slider::width() const {
    return m_barRect.width();
};


float Slider::height() const {
    return m_barRect.height();
};


std::pair<float, float> Slider::topleft() const {
    return m_barRect.topleft();
};


void Slider::setTopleft(float x, float y) {
    float currentVal {value()};

    m_barRect.setTopleft(x, y);
    m_slideRect.setCenter(m_barRect.center());

    setValue(currentVal);
};


std::pair<float, float> Slider::center() const {
    return m_barRect.center();
};


void Slider::setCenter(float x, float y) {
    float currentVal {value()};

    m_barRect.setCenter(x, y);
    m_slideRect.setCenter(x, y);

    setValue(currentVal);
};


void Slider::setX(float x) {
    float currentVal {value()};

    m_barRect.setX(x);
    m_slideRect.setCenter(m_barRect.center());

    setValue(currentVal);
};


void Slider::setY(float y) {
    float currentVal {value()};

    m_barRect.setY(y);
    m_slideRect.setCenter(m_barRect.center());

    setValue(currentVal);
};


void Slider::update(SDL_Renderer*) {
};



void Slider::setSliderPos(float x) {
    m_slideRect.setCenterX(x);
    clampSlidePos();
};


void Slider::clampSlidePos() {
    if (m_slideRect.x() < barPaddedLeft()) {
        m_slideRect.setX(barPaddedLeft());

    } else if (m_slideRect.right() > barPaddedRight()) {
        m_slideRect.setRight(barPaddedRight());
    }
};


float Slider::barPaddedLeft() const {
    return m_barRect.x() + m_slidePadding;
};


float Slider::barPaddedRight() const {
    return m_barRect.right() - m_slidePadding;
};


float Slider::roundValue(float val) const {
    return roundf(val * 1000.0f) / 1000.0f;
};