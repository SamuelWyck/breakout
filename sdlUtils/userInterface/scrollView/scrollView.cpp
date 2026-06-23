#include <vector>
#include <typeinfo>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include "../elements/iMenuElement.h"
#include "../elements/button.h"
#include "../elements/slider.h"
#include "../controlMenu/controlInput.h"
#include "../elements/elementGap.h"
#include "../../collision/fRect.h"
#include "./scrollBar.h"
#include "../../math.h"
#include "./scrollView.h"



ScrollView::ScrollView(
    float x, 
    float y, 
    float width, 
    float height, 
    float eleGap, 
    const std::vector<IMenuElement*>& elements,
    float scrollWidth, 
    float scrollMinHeight,
    const SDL_Color& scrollColor,
    const SDL_Color& scrollBgColor,
    SDL_Texture* scrollImg,
    SDL_Texture* scrollBgImg
) 
    : m_clipRect{static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height)}, 
    m_viewArea{x, y, width, height},
    m_minScrollBarHeight{scrollMinHeight}
{    
    m_totalElementsHeight = positionElements(elements, eleGap);
    m_eleOverflowHeight = m_totalElementsHeight - m_viewArea.height();
    calcScrollBar(scrollWidth, scrollColor, scrollBgColor, scrollImg, scrollBgImg);
};


void ScrollView::update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased) {
    bool mouseInScrollView {m_viewArea.pointInRect(&mousePos)};
    SDL_FPoint pos{-10, -10};
    bool mouseInvalid {true};
    if (mouseInScrollView) {
        pos = mousePos;
        mouseInvalid = false;
    }

    float relMovement {m_scrollBar.update(renderer, mousePos, mousePressed, mouseReleased)};
    if (relMovement != 0.0f) {
        relMovement = Math::remap(0.0f, m_scrollBar.maxRelMovement(), 0.0f, m_eleOverflowHeight, relMovement);
    }

    updateElements(renderer, pos, mousePressed, mouseReleased, relMovement, mouseInvalid);
};


std::vector<IMenuElement*>& ScrollView::elements() {
    return m_elements;
};



float ScrollView::positionElements(const std::vector<IMenuElement*>& eles, float eleGap) {
    float xPos {m_viewArea.x()};
    float currentY {m_viewArea.y()};

    for (IMenuElement* element : eles) {
        if (typeid(*element) == typeid(ElementGap)) {
            ElementGap* gap {static_cast<ElementGap*>(element)};
            currentY += static_cast<float>(gap->gap());
            delete element;
            continue;
        }

        element->setTopleft(xPos, currentY);
        currentY += element->height() + eleGap;
        m_elements.push_back(element);
    }

    return currentY - m_viewArea.y();
};


void ScrollView::calcScrollBar(
    float scrollWidth, 
    const SDL_Color& scrollColor, 
    const SDL_Color& scrollBgColor,
    SDL_Texture* scrollImg,
    SDL_Texture* scrollBgImg
) {
    float scrollX {m_viewArea.right()};
    float scrollY {m_viewArea.y()};
    float scrollUpperBound {m_viewArea.y()};
    float scrollLowerBound {m_viewArea.bottom()};
    float scrollHeight {m_viewArea.height()};

    if (m_totalElementsHeight > m_viewArea.height()) {
        scrollHeight -= m_eleOverflowHeight;
        if (scrollHeight < m_minScrollBarHeight) {
            scrollHeight = m_minScrollBarHeight;
        }
    }
    
    if (scrollImg == nullptr) {
        m_scrollBar = ScrollBar{
            scrollX,
            scrollY,
            scrollWidth,
            scrollHeight,
            scrollUpperBound,
            scrollLowerBound,
            scrollColor,
            scrollBgColor
        };

    } else {
        m_scrollBar = ScrollBar{
            scrollX,
            scrollY,
            scrollWidth,
            scrollHeight,
            scrollUpperBound,
            scrollLowerBound,
            scrollImg,
            scrollBgImg
        }; 
    }
};


bool ScrollView::elementInView(IMenuElement* element) {
    auto [eleX, eleY] {element->topleft()};

    if (eleY > m_viewArea.bottom() || eleY + element->height() < m_viewArea.y()) {
        return false;
    }
    return true;
};


void ScrollView::scrollElement(float relativeMovement, IMenuElement* element) {
    if (relativeMovement == 0.0f) {
        return;
    }

    auto [eleX, eleY] {element->topleft()};
    element->setY(eleY + relativeMovement);
};


void ScrollView::updateElements(
    SDL_Renderer* renderer, 
    const SDL_FPoint& mousePos, 
    bool mousePressed, 
    bool mouseReleased,
    float relMovement,
    bool mouseInvalid
) {
    SDL_SetRenderClipRect(renderer, &m_clipRect);

    for (IMenuElement* element : m_elements) {
        scrollElement(relMovement, element);
        if (!elementInView(element)) {
            continue;
        }

        if (typeid(*element) == typeid(Button)) {
            Button* btnPtr {static_cast<Button*>(element)};
            btnPtr->update(renderer, mousePos, mousePressed, mouseReleased);

        } else if (typeid(*element) == typeid(Slider)) {
            Slider* sliderPtr {static_cast<Slider*>(element)};
            bool mouseJustReleased {(mouseInvalid) ? true : mouseReleased};
            sliderPtr->update(renderer, mousePos, mousePressed, mouseJustReleased);

        } else if (typeid(*element) == typeid(ControlInput)) {
            ControlInput* input {static_cast<ControlInput*>(element)};
            input->update(renderer, mousePos, mousePressed, mouseReleased);

        } else {
            element->update(renderer);
        }
    }

    SDL_SetRenderClipRect(renderer, nullptr);
};