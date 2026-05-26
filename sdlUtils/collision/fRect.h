#ifndef UTIL_FRECT_H
#define UTIL_FRECT_H


#include <utility>
#include <iostream>
#include <SDL3/SDL_rect.h>


class FCircle;


class FRect {
    SDL_FRect m_sdlRect{};


public:
    explicit FRect();

    explicit FRect(const SDL_FRect& rect);

    FRect(float x, float y, float width, float height);

    FRect(float x, float y);
    
    explicit operator SDL_FRect() const;


    const SDL_FRect& getSDLFRect() const;
    SDL_FRect& getSDLFRect();

    // returns true if the passed rect is completely inside this rect 
    bool containsRect(const FRect* rect) const;
    bool containsRect(const SDL_FRect* rect) const;

    // pivot rect (angle) degrees clockwise about the given x y coord
    void pivot(float x, float y, double angle);

    bool hasCircleIntersection(const FCircle* circle) const;


    // start wrappers for sdl rect functions

    bool pointInRect(const SDL_FPoint* point) const;

    bool rectEmpty() const;

    bool rectsEqualEpsilon(const SDL_FRect* rect, float epsilon) const;

    bool rectsEqualEpsilon(const FRect* rect, float epsilon) const;

    bool rectsEqual(const SDL_FRect* rect) const;

    bool rectsEqual(const FRect* rect) const;

    bool hasRectIntersection(const SDL_FRect* rect) const;

    bool hasRectIntersection(const FRect* rect) const;

    bool getRectIntersection(const SDL_FRect* rect, SDL_FRect* result) const;

    bool getRectIntersection(const FRect* rect, FRect* result) const;

    bool getRectUnion(const SDL_FRect* rect, SDL_FRect* result) const;

    bool getRectUnion(const FRect* rect, FRect* result) const;

    bool getRectAndLineIntersection(float* x1, float* y1, float* x2, float* y2) const;

    // end wrappers for sdl rect functions


    // start getters and setters

    std::pair<float, float> center() const;
    void setCenter(const std::pair<float, float>& center);
    void setCenter(float centerX, float centerY);


    float centerX() const;
    void setCenterX(float centerX);


    float centerY() const;
    void setCenterY(float centerY);


    std::pair<float, float> topleft() const;
    void setTopleft(const std::pair<float, float>& topleft);
    void setTopleft(float x, float y);


    std::pair<float, float> topRight() const;
    void setTopRight(const std::pair<float, float>& topRight);
    void setTopRight(float x, float y);


    std::pair<float, float> bottomLeft() const;
    void setBottomLeft(const std::pair<float, float>& bottomLeft);
    void setBottomLeft(float x, float y);


    std::pair<float, float> bottomRight() const;
    void setBottomRight(const std::pair<float, float>& bottomRight);
    void setBottomRight(float x, float y);


    float x() const;
    void setX(float x);


    float y() const;
    void setY(float y);


    float left() const;
    void setLeft(float left);


    float right() const;
    void setRight(float right);

    float top() const;
    void setTop(float top);


    float bottom() const;
    void setBottom(float bottom);


    std::pair<float, float> midTop() const;
    void setMidTop(const std::pair<float, float>& midTop);
    void setMidTop(float topX, float topY);


    std::pair<float, float> midBottom() const;
    void setMidBottom(const std::pair<float, float>& midBottom);
    void setMidBottom(float bottomX, float bottomY);


    std::pair<float, float> midLeft() const;
    void setMidLeft(const std::pair<float, float>& midLeft);
    void setMidLeft(float leftX, float leftY);


    std::pair<float, float> midRight() const;
    void setMidRight(const std::pair<float, float>& midRight);
    void setMidRight(float rightX, float rightY);


    float width() const;
    void setWidth(float width);


    float height() const;
    void setHeight(float height);

    std::pair<float, float> size() const;
    void setSize(const std::pair<float, float>& size);
    void setSize(float width, float height);

    // end getters and setters


    friend std::ostream& operator<<(std::ostream& out, const FRect& rect);
};



#endif