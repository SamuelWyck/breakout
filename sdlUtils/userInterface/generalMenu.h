#ifndef MY_GENERAL_MENU_H
#define MY_GENERAL_MENU_H


#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include "./elements/iMenuElement.h"
#include "./elements/button.h"
#include "./elements/slider.h"
#include "./elements/elementGap.h"
#include "./elements/liveTextDisplay.h"
#include "./elements/textDisplay.h"
#include "./menuTypes.h"
#include "../collision/fRect.h"
#include "./mouse.h"


class GeneralMenu {
    std::vector<Button*> m_buttons{};
    std::vector<IMenuElement*> m_elements{};
    std::unordered_map<int, MenuCb> m_btnCallbackMap{};

    Mouse* m_mouse {nullptr};

    SDL_Texture* m_bgImage {nullptr};
    FRect m_bgImageRect{};

    std::function<void()> m_cleanupCb{nullptr};

    std::function<void()> m_musicStartCb{nullptr};
    std::function<void()> m_musicEndCb{nullptr};
    

public:
    GeneralMenu(
        float centerX,
        float yStart,
        float elementGap,
        const std::vector<IMenuElement*>& elements,
        const std::vector<MenuCb>& btnCallbacks,
        Mouse* mouse,
        SDL_Texture* bgImage=nullptr,
        float bgImageX=0,
        float bgImageY=0,
        const std::function<void()>& cleanupCallback=nullptr,
        const std::function<void()>& initCallback=nullptr,
        const std::function<void()>& musicStartCb=nullptr,
        const std::function<void()>& musicEndCb=nullptr
    ) 
        : m_mouse{mouse}, 
        m_bgImage{bgImage}, 
        m_cleanupCb{cleanupCallback}, 
        m_musicStartCb{musicStartCb}, 
        m_musicEndCb{musicEndCb}
    {
        if (m_bgImage) {
            m_bgImageRect.setTopleft(bgImageX, bgImageY);
            m_bgImageRect.setSize(static_cast<float>(m_bgImage->w), static_cast<float>(m_bgImage->h));
        }

        positionElements(elements, btnCallbacks, centerX, yStart, elementGap);

        if (initCallback) {
            initCallback();
        }
    };

    GeneralMenu(const GeneralMenu&) = delete;
    GeneralMenu(GeneralMenu&&) = delete;
    GeneralMenu& operator=(const GeneralMenu&) = delete;
    GeneralMenu& operator=(GeneralMenu&&) = delete;

    ~GeneralMenu() {
        for (Button* btnPtr : m_buttons) {
            delete btnPtr;
        }
        for (IMenuElement* elePtr : m_elements) {
            delete elePtr;
        }
    };


    MenuReturn run(SDL_Renderer* renderer, SDL_Surface* currentCanvas=nullptr) {
        m_mouse->setMouseHidden(false);

        if (m_musicStartCb) {
            m_musicStartCb();
        }

        SDL_Texture* canvas {nullptr};
        if (currentCanvas) {
            SDL_CreateTextureFromSurface(renderer, currentCanvas);
        }

        SDL_Surface* savedRender {nullptr};
        bool prepCbCanvas {false};
        bool enterCb {false};
        int clickedBtnId {};
        
        MenuReturn returnData{};
        bool running {true};

        while (running) {
            bool mousePressed {false};
            bool mouseReleased {false};

            SDL_Event event{};
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    returnData = {};
                    running = false;
                } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    m_mouse->update(event.motion.xrel, event.motion.yrel);
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    mouseReleased = true;
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    mousePressed = true;
                }
            }


            if (enterCb) {
                MenuCb& buttonCb{m_btnCallbackMap[clickedBtnId]};
                MenuReturn cbData {buttonCb(renderer, savedRender)};
                if (!cbData) {
                    running = false;
                    returnData = {};

                } else if (cbData->first > 0) {
                    cbData->first -= 1;
                    returnData = cbData;
                    running = false;
                }
            }


            SDL_FPoint mousePos{m_mouse->getPos()};

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            if (canvas) {
                SDL_RenderTexture(renderer, canvas, nullptr, nullptr);
            }
            if (m_bgImage) {
                SDL_RenderTexture(renderer, m_bgImage, nullptr, &m_bgImageRect.getSDLFRect());
            }

            
            for (Button* btnPtr : m_buttons) {
                btnPtr->update(renderer, mousePos, mousePressed, mouseReleased);
                if (btnPtr->clicked()) {
                    btnPtr->unClick();
                    clickedBtnId = btnPtr->id();
                    prepCbCanvas = true;
                }
            }

            for (IMenuElement* elePtr : m_elements) {
                if (typeid(*elePtr) == typeid(Slider)) {
                    Slider* sliderPtr {static_cast<Slider*>(elePtr)};
                    sliderPtr->update(renderer, mousePos, mousePressed, mouseReleased);
                    continue;
                }

                elePtr->update(renderer);
            }


            m_mouse->draw(renderer);

            if (prepCbCanvas) {
                prepCbCanvas = false;
                enterCb = true;
                savedRender = SDL_RenderReadPixels(renderer, nullptr);
            }

            SDL_RenderPresent(renderer);
        }


        SDL_DestroyTexture(canvas);
        SDL_DestroySurface(savedRender);

        if (m_cleanupCb) {
            m_cleanupCb();
        }
        if (m_musicEndCb) {
            m_musicEndCb();
        }

        return returnData;
    };


private:
    void positionElements(
        const std::vector<IMenuElement*>& elements, 
        const std::vector<MenuCb>& btnCallbacks,
        float eleCenterX, float eleStartY, float eleGap
    ) {
        
        int btnNum {0};
        
        float currentY {eleStartY};

        for (IMenuElement* ele : elements) {
            if (typeid(ele) == typeid(ElementGap)) {
                ElementGap* gap {static_cast<ElementGap*>(ele)};
                currentY += gap->gap();
                continue;
            }

            float centerY {currentY + (ele->height() / 2)};
            ele->setCenter(eleCenterX, centerY);
            currentY += eleGap + ele->height();

            if (typeid(*ele) == typeid(Button)) {
                Button* button {static_cast<Button*>(ele)};
                button->setId(btnNum);
                m_btnCallbackMap[btnNum] = btnCallbacks[btnNum];
                m_buttons.push_back(button);

            } else {
                m_elements.push_back(ele);
            }
        }
    };
};



#endif