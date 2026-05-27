#ifndef MENU_ELEMENT_INTERFACE_H
#define MENU_ELEMENT_INTERFACE_H


#include <utility>
#include <SDL3/SDL_render.h>


class IMenuElement {
public:
    virtual ~IMenuElement() = default;

    virtual float width() const = 0;
    virtual float height() const = 0;

    virtual std::pair<float, float> topleft() const = 0;
    virtual void setTopleft(float x, float y) = 0;

    virtual std::pair<float, float> center() const = 0;
    virtual void setCenter(float x, float y) = 0;

    virtual void setX(float x) = 0;
    virtual void setY(float y) = 0;

    virtual void update(SDL_Renderer* renderer) = 0;
};



#endif