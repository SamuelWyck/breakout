#ifndef MY_SELECT_MENU_H
#define MY_SELECT_MENU_H


#include <vector>
#include <utility>
#include <string>
#include <string_view>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "./elements/button.h"
#include "./menuTypes.h"
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

    std::string m_saveFilePath{};

    int m_minBtnIdx {};
    int m_maxBtnIdx {};
    int m_btnIdxChange {};


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
        SDL_Texture* choiceLockedImg=nullptr,
        std::string_view saveFilePath=""
    );

    SelectMenu(const SelectMenu&) = delete;
    SelectMenu(SelectMenu&&) = delete;
    SelectMenu& operator=(const SelectMenu&) = delete;
    SelectMenu& operator=(SelectMenu&&) = delete;
    
    ~SelectMenu();


    MenuReturn run(SDL_Renderer* renderer, int framerate, SDL_Surface* bgCanvas=nullptr);


    int runUpdate(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased);


    void setHighestUnlockedBtnId(int highestUnlockedId);


    int highestUnlockedBtnId() const;


    void pageToTop();

    

private:
    void loadHighestUnlockedId();


    void saveHighestUnlockedId();


    void drawLockedImg(SDL_Renderer* renderer, Button* btnPtr);


    bool btnUnlocked(int btnId);


    void pageButtons(bool pageUp);


    void positionPageAndExitBtns(
        Button* exitBtn, 
        Button* pageUpBtn, 
        Button* pageDownBtn, 
        float colHeight, 
        float btnGap, 
        float centerX, 
        float centerY
    );


    float positionBtns(int numRows, int numCols, float centerX, float centerY, float btnGap);

    
    std::pair<float, float> maxBtnSize();
};



#endif