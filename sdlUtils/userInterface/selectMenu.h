#ifndef MY_SELECT_MENU_H
#define MY_SELECT_MENU_H


#include <vector>
#include <utility>
#include <SDL3/SDL_render.h>
#include "./elements/button.h"
#include "./menuTypes.h"


class SelectMenu {
    std::vector<Button*> m_buttons{};

    SDL_Texture* m_bgImage {nullptr};


public:
    SelectMenu(
        float centerX, 
        float centerY, 
        int numRows, 
        int numCols, 
        float btnGap,
        const std::vector<Button*>& buttons,
        SDL_Texture* bgImage=nullptr
    ) 
        : m_buttons{buttons}, m_bgImage{bgImage}
    {

    };


private:
    void positionBtns(int numRows, int numCols, float centerX, float centerY, float btnGap) {
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