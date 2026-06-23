#ifndef MY_SCROLLVIEW_H
#define MY_SCROLLVIEW_H


#include <vector>
#include <utility>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include "../elements/iMenuElement.h"
#include "../../collision/fRect.h"
#include "./scrollBar.h"


class ScrollView {
    SDL_Rect m_clipRect{};
    FRect m_viewArea{};

    ScrollBar m_scrollBar{};

    float m_scrollBarGap {};
    float m_totalElementsHeight {};
    float m_eleOverflowHeight {};
    float m_minScrollBarHeight {20};

    std::vector<IMenuElement*> m_elements{};


public:
    ScrollView(
        float x, 
        float y, 
        float width, 
        float height, 
        float eleGap, 
        float scrollGap,
        const std::vector<IMenuElement*>& elements,
        float scrollWidth, 
        float scrollMinHeight,
        const SDL_Color& scrollColor,
        const SDL_Color& scrollBgColor,
        SDL_Texture* scrollImg=nullptr,
        SDL_Texture* scrollBgImg=nullptr
    );

    ScrollView(const ScrollView&) = delete;
    ScrollView(ScrollView&&) = delete;
    ScrollView& operator=(const ScrollView&) = delete;
    ScrollView& operator=(ScrollView&&) = delete;
    ~ScrollView() = default;

    void update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased);


    std::vector<IMenuElement*>& elements();


    void setTopleft(float x, float y);


    std::pair<float, float> topleft() const;


    void setWidth(float newWidth);


    float width() const;

    // Set scroll position in the relative range 0-100 where 0 is the top.
    void setScrollPos(float scrollPos) {
        float relativeMovement {m_scrollBar.setBarPos(scrollPos)};
        float realMovementDelta {remapToRealRange(relativeMovement)};

        for (IMenuElement* elePtr : m_elements) {
            scrollElement(realMovementDelta, elePtr);
        }
    };



private:
    float positionElements(const std::vector<IMenuElement*>& eles, float eleGap);

    void calcScrollBar(
        float scrollWidth, 
        const SDL_Color& scrollColor, 
        const SDL_Color& scrollBgColor,
        SDL_Texture* scrollImg,
        SDL_Texture* scrollBgImg
    );

    bool elementInView(IMenuElement* element);

    void scrollElement(float relativeMovement, IMenuElement* element);

    void updateElements(
        SDL_Renderer* renderer, 
        const SDL_FPoint& mousePos, 
        bool mousePressed, 
        bool mouseReleased, 
        float relMovement,
        bool mouseInvalid
    );

    float remapToRealRange(float relMovement);
};



#endif