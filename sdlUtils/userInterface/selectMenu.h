#ifndef MY_SELECT_MENU_H
#define MY_SELECT_MENU_H


#include <vector>
#include <utility>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include "./elements/button.h"
#include "./menuTypes.h"
#include "../frameCap.h"
#include "./mouse.h"
#include "../collision/fRect.h"


class SelectMenu {
    Mouse* m_mouse {nullptr};

    std::vector<Button*> m_buttons{};
    std::vector<Button*> m_interfaceBtns{};

    int m_exitId {0};
    int m_pageUpId {1};
    int m_pageDownId {2};

    SDL_Texture* m_bgImage {nullptr};
    FRect m_bgImageRect{};

    SDL_Texture* m_btnLockedImg {nullptr};
    FRect m_lockedImgRect{};
    int m_highestUnlockedId {0};

    int m_minBtnIdx {};
    int m_maxBtnIdx {};
    int m_idxChange {};


public:
    SelectMenu(
        Mouse* mouse,
        float centerX, 
        float centerY, 
        int numRows, 
        int numCols, 
        float btnGap,
        const std::vector<Button*>& buttons,
        Button* exitButton,
        Button* pageUpButton,
        Button* pageDownButton,
        SDL_Texture* bgImage=nullptr,
        SDL_Texture* choiceLockedImg=nullptr
    ) 
        : m_mouse{mouse}, 
        m_buttons{buttons}, 
        m_bgImage{bgImage}, 
        m_btnLockedImg{choiceLockedImg}
    {
        float colHeight {positionBtns(numRows, numCols, centerX, centerY, btnGap)};
        positionPageAndExitBtns(exitButton, pageUpButton, pageDownButton, colHeight, btnGap, centerX, centerY);

        if (bgImage != nullptr) {
            m_bgImageRect.setSize(static_cast<float>(bgImage->w), static_cast<float>(bgImage->h));
            m_bgImageRect.setCenter(centerX, centerY);
        }
        if (choiceLockedImg) {
            m_lockedImgRect.setSize(static_cast<float>(choiceLockedImg->w), static_cast<float>(choiceLockedImg->h));
        }
    };

    SelectMenu(const SelectMenu&) = delete;
    SelectMenu(SelectMenu&&) = delete;
    SelectMenu& operator=(const SelectMenu&) = delete;
    SelectMenu& operator=(SelectMenu&&) = delete;
    
    ~SelectMenu() {
        for (Button* btn : m_buttons) {
            delete btn;
        }

        for (Button* btn : m_interfaceBtns) {
            delete btn;
        }
    };


    MenuReturn run(SDL_Renderer* renderer, int framerate, SDL_Surface* bgCanvas=nullptr) {
        SDL_Texture* bgTexture {nullptr};
        if (bgCanvas != nullptr) {
            bgTexture = SDL_CreateTextureFromSurface(renderer, bgCanvas);
        }

        bool running {true};
        FrameCap frameCap{};
        MenuReturn returnVal{};

        int btnsLength {static_cast<int>(m_buttons.size())};

        while (running) {
            bool mousePressed {false};
            bool mouseReleased {false};

            SDL_Event event{};
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    running = false;
                    returnVal = {};
                } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    m_mouse->update(event.motion.xrel, event.motion.yrel);
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mousePressed = true;
                    }
                } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouseReleased = true;
                    }
                }
            }


            SDL_FPoint mousePos{m_mouse->getPos()};

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (bgTexture) {
                SDL_RenderTexture(renderer, bgTexture, nullptr, nullptr);
            }
            if (m_bgImage) {
                SDL_RenderTexture(renderer, m_bgImage, nullptr, &m_bgImageRect.getSDLFRect());
            }

            for (Button* btnPtr : m_interfaceBtns) {
                btnPtr->update(renderer, mousePos, mousePressed, mouseReleased);
                if (btnPtr->clicked()) {
                    btnPtr->unclick();

                    if (btnPtr->id() == m_exitId) {
                        returnVal = {{0, -1}};
                        running = false;
                    } else {
                        pageButtons(btnPtr->id() == m_pageUpId);
                    }
                }
            }

            for (int i {m_minBtnIdx}; i < m_maxBtnIdx; i += 1) {
                if (i >= btnsLength) {
                    break;
                }

                Button* btnPtr {m_buttons[i]};
                btnPtr->update(renderer, mousePos, mousePressed, mouseReleased);
                drawLockedImg(renderer, btnPtr);

                if (btnPtr->clicked()) {
                    btnPtr->unclick();

                    if (btnUnlocked(btnPtr->id())) {
                        returnVal = {{0, btnPtr->id()}};
                        running = false;
                    }
                }
            }

            m_mouse->draw(renderer);

            SDL_RenderPresent(renderer);
            frameCap.capFrames(framerate);
        }


        SDL_DestroyTexture(bgTexture);
        return returnVal;
    };


    int runUpdate(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased) {
        if (m_bgImage) {
            SDL_RenderTexture(renderer, m_bgImage, nullptr, &m_bgImageRect.getSDLFRect());
        }

        int returnVal {-1};

        for (Button* btnPtr : m_interfaceBtns) {
            btnPtr->update(renderer, mousePos, mousePressed, mouseReleased);
            if (btnPtr->clicked()) {
                btnPtr->unclick();
                if (btnPtr->id() == m_exitId) {
                    returnVal = -2;
                } else {
                    pageButtons(btnPtr->id() == m_pageUpId);
                }
            }
        }

        int btnsLength {static_cast<int>(m_buttons.size())};
        for (int i {m_minBtnIdx}; i < m_maxBtnIdx; i += 1) {
            if (i >= btnsLength) {
                break;
            }

            Button* btnPtr {m_buttons[i]};
            btnPtr->update(renderer, mousePos, mousePressed, mouseReleased);
            drawLockedImg(renderer, btnPtr);

            if (btnPtr->clicked()) {
                btnPtr->unclick();
                if (btnUnlocked(btnPtr->id())) {
                    returnVal = btnPtr->id();
                }
            }
        }

        return returnVal;
    };


    void setHighestUnlockedBtnId(int highestUnlockedId) {
        m_highestUnlockedId = highestUnlockedId;
    };


    int highestUnlockedBtnId() const {
        return m_highestUnlockedId;
    };



private:
    void drawLockedImg(SDL_Renderer* renderer, Button* btnPtr) {
        if (btnUnlocked(btnPtr->id())) {
            return;
        }

        m_lockedImgRect.setCenter(btnPtr->center());
        SDL_RenderTexture(renderer, m_btnLockedImg, nullptr, &m_lockedImgRect.getSDLFRect());
    };


    bool btnUnlocked(int btnId) {
        if (!m_btnLockedImg) {
            return true;
        }

        return btnId <= m_highestUnlockedId;
    };


    void pageButtons(bool pageUp) {
        if (pageUp && m_minBtnIdx == 0) {
            return;
        }
        if (!pageUp && m_maxBtnIdx >= static_cast<int>(m_buttons.size())) {
            return;
        }

        int idxChange {(pageUp) ? -m_idxChange : m_idxChange};
        m_minBtnIdx += idxChange;
        m_maxBtnIdx += idxChange;
    };


    void positionPageAndExitBtns(
        Button* exitBtn, 
        Button* pageUpBtn, 
        Button* pageDownBtn, 
        float colHeight, 
        float btnGap, 
        float centerX, 
        float centerY
    ) {
        float btnsAreaTop {centerY - (colHeight / 2.0f)};
        float btnsAreaBottom {centerY + (colHeight / 2.0f)};

        pageUpBtn->setCenter(centerX, btnsAreaTop - btnGap - (pageUpBtn->height() / 2.0f));
        pageUpBtn->setId(m_pageUpId);
        m_interfaceBtns.push_back(pageUpBtn);

        pageDownBtn->setCenter(centerX, btnsAreaBottom + btnGap + (pageDownBtn->height() / 2.0f));
        pageDownBtn->setId(m_pageDownId);
        m_interfaceBtns.push_back(pageDownBtn);

        auto [pageDownX, pageDownY] {pageDownBtn->topleft()};
        exitBtn->setCenter(
            centerX, 
            pageDownY + pageDownBtn->height() + (btnGap * 2) + (exitBtn->height() / 2.0f)
        );
        exitBtn->setId(m_exitId);
        m_interfaceBtns.push_back(exitBtn);
    };


    float positionBtns(int numRows, int numCols, float centerX, float centerY, float btnGap) {
        auto [maxBtnWidth, maxBtnHeight] {maxBtnSize()};
        float rowWidth {maxBtnWidth * numCols + (btnGap * (numCols - 1))};
        float colHeight {maxBtnHeight * numRows + (btnGap * (numRows - 1))};

        float firstRowY {centerY - (colHeight / 2.0f)};
        float firstColX {centerX - (rowWidth / 2.0f)};


        int btnIdx {0};
        int btnsLength {static_cast<int>(m_buttons.size())};
        while (btnIdx < btnsLength) {
            float currentY {firstRowY};

            for (int row {0}; row < numRows; row += 1) {
                float currentX {firstColX};

                for (int col {0}; col < numCols; col += 1) {
                    if (btnIdx >= static_cast<int>(m_buttons.size())) {
                        break;
                    }
    
                    Button* btn {m_buttons[btnIdx]};
    
                    float btnCenterX {currentX + (maxBtnWidth / 2.0f)};
                    float btnCenterY {currentY + (maxBtnHeight / 2.0f)};
                    btn->setCenter(btnCenterX, btnCenterY);
                    btn->setId(btnIdx);

                    currentX += maxBtnWidth + btnGap;
                    btnIdx += 1;
                }

                currentY += maxBtnHeight + btnGap;
            }
        }

        m_maxBtnIdx = numRows * numCols;
        m_idxChange = m_maxBtnIdx;
        return colHeight;
    };

    
    std::pair<float, float> maxBtnSize() {
        float maxWidth {0};
        float maxHeight {0};

        for (Button* btnPtr : m_buttons) {
            float btnWidth {btnPtr->width()};
            maxWidth = (maxWidth < btnWidth) ? btnWidth : maxWidth;

            float btnHeight {btnPtr->height()};
            maxHeight = (maxHeight < btnHeight) ? btnHeight : maxHeight;
        }

        return std::pair<float, float>{maxWidth, maxHeight};
    };
};



#endif