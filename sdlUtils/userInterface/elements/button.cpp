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


void Button::unclick() {
    m_clicked = false;
};


int Button::id() const {
    return m_id;
};


void Button::setId(int newId) {
    m_id = newId;
};


// Methods for menu element interface

void Button::update(SDL_Renderer*) {
};

float Button::width() const {
    if (m_hoverImg == nullptr) {
        return m_imgRect.width();
    }
    float imgWidth {m_imgRect.width()};
    float hvrImgWidth {m_hoverImgRect.width()};
    return (imgWidth < hvrImgWidth) ? hvrImgWidth : imgWidth;
};

float Button::height() const {
    if (m_hoverImg == nullptr) {
        return m_imgRect.height();
    }
    float imgHeight {m_imgRect.height()};
    float hvrImgHeight {m_hoverImgRect.height()};
    return (imgHeight < hvrImgHeight) ? hvrImgHeight : imgHeight;
};

std::pair<float, float> Button::topleft() const {
    return m_imgRect.topleft();
};

void Button::setTopleft(float x, float y) {
    m_imgRect.setTopleft(x, y);
    if (m_hoverImg != nullptr) {
        m_hoverImgRect.setCenter(m_imgRect.center());
    }
};

std::pair<float, float> Button::center() const {
    return m_imgRect.center();
};

void Button::setCenter(float x, float y) {
    m_imgRect.setCenter(x, y);
    if (m_hoverImg != nullptr) {
        m_hoverImgRect.setCenter(m_imgRect.center());
    }
};

void Button::setX(float x) {
    m_imgRect.setX(x);
    if (m_hoverImg != nullptr) {
        m_hoverImgRect.setCenter(m_imgRect.center());
    }
};

void Button::setY(float y) {
    m_imgRect.setY(y);
    if (m_hoverImg != nullptr) {
        m_hoverImgRect.setCenter(m_imgRect.center());
    }
};