#ifndef MY_BUTTON_MENU_H
#define MY_BUTTON_MENU_H


#include <functional>
#include <utility>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include "./elements/button.h"
#include "./menuTypes.h"
#include "./mouse.h"
#include "../clock.h"
#include "../collision/fRect.h"


class ButtonMenu {
    std::vector<Button*> m_buttons{};
    std::unordered_map<int, MenuCb> m_buttonCbMap{};
    
    float m_btnX {};
    float m_btnY {};
    float m_btnGap {};
    
    Mouse* m_mouseManger {nullptr};

    SDL_Texture* m_bgImage {nullptr};
    FRect m_bgImageRect{};


public:
    ButtonMenu(
        const std::vector<Button*>& buttons, 
        const std::vector<MenuCb>& btnCallbacks,
        float buttonX,
        float buttonY,
        float buttonGap,
        Mouse* mouseManager,
        SDL_Texture* bgImage=nullptr,
        float bgImageX=0,
        float bgImageY=0
    ) 
        : m_buttons{buttons}, 
        m_btnX{buttonX}, 
        m_btnY{buttonY}, 
        m_btnGap{buttonGap},
        m_mouseManger{mouseManager},
        m_bgImage{bgImage}
    {   
        if (m_buttons.size() != btnCallbacks.size()) {
            throw std::runtime_error("There must be the same number of callbacks and buttons.\n");
        }

        if (m_bgImage) {
            m_bgImageRect.setSize(static_cast<float>(m_bgImage->w), static_cast<float>(m_bgImage->h));
            m_bgImageRect.setTopleft(bgImageX, bgImageY);
        }

        positionButtons(btnCallbacks);
    };

    ButtonMenu(const ButtonMenu&) = delete;
    ButtonMenu(ButtonMenu&&) = delete;
    ButtonMenu& operator=(const ButtonMenu&) = delete;
    ButtonMenu& operator=(ButtonMenu&&) = delete;

    ~ButtonMenu() {
        for (Button* btnPtr : m_buttons) {
            delete btnPtr;
        }
    };


    MenuReturn run(SDL_Renderer* renderer, SDL_Surface* currentCanvas=nullptr) {
        bool running {true};
        m_mouseManger->setMouseHidden(false);

        SDL_Texture* canvasTexture {nullptr};
        if (currentCanvas) {
            canvasTexture = SDL_CreateTextureFromSurface(renderer, currentCanvas);
        }

        SDL_Surface* savedRender {nullptr};

        bool prepCanvas {false};
        bool enterCb {false};
        int clickedBtnId {};

        while (running) {
            bool leftMousePressed {false};
            bool leftMouseReleased {false};

            SDL_Event event{};
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    m_mouseManger->update(event.motion.xrel, event.motion.yrel);
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    leftMousePressed = true;
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    leftMouseReleased = true;
                }
            }


            if (enterCb) {
                enterCb = false;

                MenuCb& buttonCb {m_buttonCbMap[clickedBtnId]};
                MenuReturn cbReturn{buttonCb(renderer, savedRender)};
                if (!cbReturn) {
                    SDL_DestroyTexture(canvasTexture);
                    SDL_DestroySurface(savedRender);
                    return MenuReturn{};
                }

                auto [menuLevels, data] = *cbReturn;
                if (menuLevels != 0) {
                    cbReturn->first -= 1;
                    SDL_DestroyTexture(canvasTexture);
                    SDL_DestroySurface(savedRender);
                    return cbReturn;
                }
            }


            SDL_FPoint mousePos{m_mouseManger->getPos()};

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (canvasTexture) {
                SDL_RenderTexture(renderer, canvasTexture, nullptr, nullptr);
            }
            if (m_bgImage) {
                SDL_RenderTexture(renderer, m_bgImage, nullptr, &m_bgImageRect.getSDLFRect());
            }

            for (Button* btnPtr : m_buttons) {
                btnPtr->update(renderer, mousePos, leftMousePressed, leftMouseReleased);
                if (btnPtr->clicked()) {
                    btnPtr->unClick();
                    clickedBtnId = btnPtr->id();
                    prepCanvas = true;
                }
            }

            m_mouseManger->draw(renderer);

            if (prepCanvas) {
                prepCanvas = false;
                enterCb = true;
                SDL_DestroySurface(savedRender);
                savedRender = SDL_RenderReadPixels(renderer, nullptr);
            }

            SDL_RenderPresent(renderer);
        }

        SDL_DestroyTexture(canvasTexture);
        SDL_DestroySurface(savedRender);
        return MenuReturn{};
    };


    int runUpdate(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased) {
        if (m_bgImage) {
            SDL_RenderTexture(renderer, m_bgImage, nullptr, &m_bgImageRect.getSDLFRect());
        }

        int data {-1};
        for (Button* btnPtr : m_buttons) {
            btnPtr->update(renderer, mousePos, mousePressed, mouseReleased);
            if (btnPtr->clicked()) {
                btnPtr->unClick();
                MenuCb& buttonCb {m_buttonCbMap[btnPtr->id()]};
                MenuReturn returnVal {buttonCb(renderer, nullptr)};
                data = returnVal->second;
            }
        }

        return data;
    };



private:
    void positionButtons(std::vector<MenuCb> btnCallbacks) {
        int length {static_cast<int>(m_buttons.size())};
        float currentY {m_btnY};
        float maxBtnHeight {getMaxButtonHeight()};

        for (int i {0}; i < length; i += 1) {
            Button* btnPtr {m_buttons[i]};
            auto& callback {btnCallbacks[i]};

            m_buttonCbMap[i] = callback;
            btnPtr->setId(i);
            btnPtr->setX(m_btnX);
            
            auto btnCenter {btnPtr->center()};
            btnPtr->setCenter(btnCenter.first, currentY + (maxBtnHeight / 2.0f));

            currentY += m_btnGap + maxBtnHeight;
        }
    };


    float getMaxButtonHeight() {
        float maxHeight {0};
        for (Button* btnPtr : m_buttons) {
            if (maxHeight < btnPtr->height()) {
                maxHeight = btnPtr->height();
            }
        }
        return maxHeight;
    };
};



#endif