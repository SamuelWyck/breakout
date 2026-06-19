#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include "../../collision/fRect.h"
#include "../../math.h"
#include "./scrollBar.h"




ScrollBar::ScrollBar(
    float x, 
    float y, 
    float width, 
    float height, 
    float upperBound, 
    float lowerBound, 
    SDL_Texture* img,
    SDL_Texture* bgImg
)
    : m_rect{x, y, width, height}, m_img{img}, m_bgImg{bgImg}
{   
    calcBgRect(upperBound, lowerBound);
    m_maxBarTravelDis = maxBarCenterMoveDis();
};

ScrollBar::ScrollBar(
    float x, 
    float y, 
    float width, 
    float height, 
    float upperBound, 
    float lowerBound, 
    const SDL_Color& color, 
    const SDL_Color& bgColor
) 
    : m_rect{x, y, width, height}, m_color{color}, m_bgColor{bgColor}
{
    calcBgRect(upperBound, lowerBound);
    m_maxBarTravelDis = maxBarCenterMoveDis();
};

ScrollBar::ScrollBar() {
    m_maxBarTravelDis = maxBarCenterMoveDis();
};


// Updates the scrollbar and returns the amount of relative movement in the range 0.0f-100.0f
float ScrollBar::update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased) {
    bool mouseHovering {m_rect.pointInRect(&mousePos)};
    bool mouseHoveringBg {m_bgRect.pointInRect(&mousePos)};

    if ((mouseHovering || mouseHoveringBg) && mousePressed) {
        m_scrollActive = true;
    } else if (mouseReleased) {
        m_scrollActive = false;
    }

    float relativeMovement {0.0f};
    if (m_scrollActive) {
        relativeMovement = setBarCenterY(mousePos.y);
    }

    draw(renderer);
    return relativeMovement;
};


float ScrollBar::maxRelMovement() const {
    return m_maxDstValue;
};



float ScrollBar::setBarCenterY(float yPos) {
    float oldCenterY {m_rect.centerY()};
    m_rect.setCenterY(yPos);

    if (m_rect.y() < m_bgRect.top()) {
        m_rect.setY(m_bgRect.top());
    } else if (m_rect.bottom() > m_bgRect.bottom()) {
        m_rect.setY(m_bgRect.bottom() - m_rect.height());
    }

    float relativeMovement {oldCenterY - m_rect.centerY()};
    if (m_maxBarTravelDis != 0.0f) {
        relativeMovement = Math::remap(0.0f, m_maxBarTravelDis, 0.0f, m_maxDstValue, relativeMovement);
    }
    return relativeMovement;
};


float ScrollBar::maxBarCenterMoveDis() {
    float lowBound {m_bgRect.bottom() - (m_rect.height() / 2.0f)};
    float highBound {m_bgRect.top() + (m_rect.height() / 2.0f)};
    return lowBound - highBound;
};


void ScrollBar::calcBgRect(float upperBound, float lowerBound) {
    m_bgRect.setTopleft(m_rect.topleft());
    float height {lowerBound - upperBound};
    m_bgRect.setSize(m_rect.width(), height);
};


void ScrollBar::draw(SDL_Renderer* renderer) {
    if (!m_img) {
        if (validBgColor()) {
            SDL_SetRenderDrawColor(renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
            SDL_RenderFillRect(renderer, &m_bgRect.getSDLFRect());
        }

        SDL_SetRenderDrawColor(renderer, m_color.r, m_color.g, m_color.b, m_color.a);
        SDL_RenderFillRect(renderer, &m_rect.getSDLFRect());

    } else {
        if (m_bgImg != nullptr) {
            SDL_RenderTexture(renderer, m_bgImg, nullptr, &m_bgRect.getSDLFRect());
        }

        SDL_RenderTexture(renderer, m_img, nullptr, &m_rect.getSDLFRect());
    }
};


bool ScrollBar::validBgColor() {
    return m_bgColor.r != 0 && m_bgColor.g != 0 && m_bgColor.g != 0 && m_bgColor.a != 0;
};