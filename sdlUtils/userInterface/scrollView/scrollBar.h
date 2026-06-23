#ifndef MY_SCROLLBAR_H
#define MY_SCROLLBAR_H


#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include "../../collision/fRect.h"


class ScrollBar {
    FRect m_rect{};
    FRect m_bgRect{};

    float m_maxBarTravelDis {};
    float m_maxDstValue {100.0f};

    SDL_Texture* m_img {nullptr};
    SDL_Texture* m_bgImg {nullptr};

    SDL_Color m_color{0, 0, 0, 255};
    SDL_Color m_bgColor{};

    bool m_scrollActive {false};


public:
    ScrollBar(
        float x, 
        float y, 
        float width, 
        float height, 
        float upperBound, 
        float lowerBound, 
        SDL_Texture* img,
        SDL_Texture* bgImg=nullptr
    );

    ScrollBar(
        float x, 
        float y, 
        float width, 
        float height, 
        float upperBound, 
        float lowerBound, 
        const SDL_Color& color, 
        const SDL_Color& bgColor=SDL_Color{0, 0, 0, 0}
    );

    ScrollBar();

    ScrollBar(const ScrollBar&) = default;
    ScrollBar(ScrollBar&&) = default;
    ScrollBar& operator=(const ScrollBar&) = default;
    ScrollBar& operator=(ScrollBar&&) = default;
    ~ScrollBar() = default;


    // Updates the scrollbar and returns the amount of relative movement in the range 0.0f-100.0f
    float update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased);


    float maxRelMovement() const;


    void setX(float x);


    void setY(float y);



private:
    float setBarCenterY(float yPos);

    float maxBarCenterMoveDis();

    void calcBgRect(float upperBound, float lowerBound);

    void draw(SDL_Renderer* renderer);

    bool validBgColor();
};



#endif