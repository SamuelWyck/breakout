#include <utility>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "./button.h"



Button::Button(float x, float y, SDL_Texture* img, SDL_Texture* hoverImg, int id) 
    : m_img{img}, m_hoverImg{hoverImg}, m_id{id} 
{
    m_imgRect.setTopleft(x, y);
    m_imgRect.setSize(static_cast<float>(m_img->w), static_cast<float>(m_img->h));

    if (m_hoverImg) {
        m_hoverImgRect.setSize(static_cast<float>(m_hoverImg->w), static_cast<float>(m_hoverImg->h));
        m_hoverImgRect.setCenter(m_imgRect.center());
    }
};



void Button::update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mouseJustPressed, bool mouseJustReleased) {
    m_clicked = false;

    bool hovering {m_imgRect.pointInRect(&mousePos)};
    if (hovering && m_hoverImg != nullptr) {
        m_activeImg = m_hoverImg;
        m_activeRect = &m_hoverImgRect;
    } else {
        m_activeImg = m_img;
        m_activeRect = &m_imgRect;
    }

    if (hovering && mouseJustPressed) {
        m_mousePressed = true;

    } else if (hovering && m_mousePressed && mouseJustReleased) {
        m_mousePressed = false;
        m_clicked = true;

    } else if (mouseJustReleased) {
        m_mousePressed = false;
    }


    SDL_RenderTexture(renderer, m_activeImg, nullptr, &m_activeRect->getSDLFRect());
};


bool Button::clicked() const {
    return m_clicked;
};


void Button::unClick() {
    m_clicked = false;
};


int Button::id() const {
    return m_id;
};


// Methods for menu element interface

void Button::update(SDL_Renderer*) {
};

float Button::width() const {
    return m_imgRect.width();
};

float Button::height() const {
    return m_imgRect.height();
};

std::pair<float, float> Button::topleft() const {
    return m_imgRect.topleft();
};

void Button::setTopleft(float x, float y) {
    m_imgRect.setTopleft(x, y);
};

std::pair<float, float> Button::center() const {
    return m_imgRect.center();
};

void Button::setCenter(float x, float y) {
    m_imgRect.setCenter(x, y);
};

void Button::setX(float x) {
    m_imgRect.setX(x);
};

void Button::setY(float y) {
    m_imgRect.setY(y);
};