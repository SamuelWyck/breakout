#ifndef MENU_ELEMENT_GAP_H
#define MENU_ELEMENT_GAP_H


#include <utility>
#include <SDL3/SDL_render.h>
#include "./iMenuElement.h"


class ElementGap : public IMenuElement {
    int m_gap {};


public:
    ElementGap();
    ElementGap(int gap);

    
    int gap() const;


    void update(SDL_Renderer*) override;
    float width() const override;
    float height() const override;
    std::pair<float, float> topleft() const override;
    void setTopleft(float, float) override;
    std::pair<float, float> center() const override;
    void setCenter(float, float) override;
    void setX(float) override;
    void setY(float) override;
};



#endif