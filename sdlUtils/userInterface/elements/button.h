#ifndef MY_BUTTON_H
#define MY_BUTTON_H


#include <utility>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "./iMenuElement.h"
#include "../../collision/fRect.h"


class Button : public IMenuElement {
    FRect m_imgRect{};
    FRect m_hoverImgRect{};
    FRect* m_activeRect {nullptr};

    SDL_Texture* m_img {nullptr};
    SDL_Texture* m_hoverImg {nullptr};
    SDL_Texture* m_activeImg {nullptr};

    int m_id {};

    bool m_clicked {false};
    bool m_mousePressed {false};


public:
    Button(float x, float y, SDL_Texture* img, SDL_Texture* hoverImg=nullptr, int id=-1);

    Button(const Button&) = default;
    Button(Button&&) = default;
    Button& operator=(const Button&) = default;
    Button& operator=(Button&&) = default;
    ~Button() = default;



    void update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mouseJustPressed, bool mouseJustReleased);


    bool clicked() const;
    void unClick();


    int id() const;
    void setId(int newId);


    // Methods for menu element interface

    void update(SDL_Renderer*) override;

    float width() const override;
    float height() const override;

    std::pair<float, float> topleft() const override;
    void setTopleft(float x, float y) override;

    std::pair<float, float> center() const override;
    void setCenter(float x, float y) override;

    void setX(float x) override;
    void setY(float y) override;
};



#endif