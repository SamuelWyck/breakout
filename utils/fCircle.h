#ifndef MY_CIRCLE_H
#define MY_CIRCLE_H


#include <utility>
#include "SDL3/SDL_rect.h"
#include "./fRect.h"


class FCircle {
    float m_centerX {};
    float m_centerY {};
    float m_radius {};


public:
    FCircle();
    FCircle(float centerX, float centerY, float radius);
    FCircle(float centerX, float centerY);


    std::pair<float, float> center() const;
    void setCenter(const std::pair<float, float>& center);
    void setCenter(float centerX, float centerY);

    float centerX() const;
    void setCenterX(float centerX);

    float centerY() const;
    void setCenterY(float centerY);;

    float radius() const;
    void setRadius(float radius);


    SDL_FRect getSDLFRect() const;
    FRect getFRect() const;
    operator SDL_FRect() const;
    operator FRect() const;


    bool hasCircleIntersection(const FCircle* circle) const;
    bool hasRectIntersection(const FRect* rect) const;
    bool hasRectIntersection(const SDL_FRect* rect) const;

    // pivot circle (angle) degrees clockwise about the given x y coord
    void pivot(float x, float y, double angle);
};



#endif