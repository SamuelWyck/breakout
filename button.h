#ifndef MY_BUTTON_H
#define MY_BUTTON_H


#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "./sdlUtils/collision/fRect.h"


class Button {
    FRect m_imgRect{};
    FRect m_hoverImgRect{};
    FRect* m_activeRect {nullptr};

    SDL_Texture* m_img {nullptr};
    SDL_Texture* m_hoverImg {nullptr};
    SDL_Texture* m_activeImg {nullptr};


    bool m_clicked {false};


public:
    Button(float x, float y, SDL_Texture* img, SDL_Texture* hoverImg=nullptr) : m_img{img}, m_hoverImg{hoverImg} {
        m_imgRect.setTopleft(x, y);
        m_imgRect.setSize(static_cast<float>(m_img->w), static_cast<float>(m_img->h));

        if (m_hoverImg) {
            m_hoverImgRect.setSize(static_cast<float>(m_hoverImg->w), static_cast<float>(m_hoverImg->h));
            m_hoverImgRect.setCenter(m_imgRect.center());
        }
    };


    void update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool, bool) {
        m_clicked = false;

        bool hovering {m_imgRect.pointInRect(&mousePos)};
        if (hovering && m_hoverImg != nullptr) {
            m_activeImg = m_hoverImg;
            m_activeRect = &m_hoverImgRect;
        } else {
            m_activeImg = m_img;
            m_activeRect = &m_imgRect;
        }


        SDL_RenderTexture(renderer, m_activeImg, nullptr, &m_activeRect->getSDLFRect());
    };


    bool clicked() const {
        return m_clicked;
    };
};



#endif