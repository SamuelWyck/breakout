#ifndef MY_BUTTON_MENU_H
#define MY_BUTTON_MENU_H


#include <functional>
#include <unordered_map>
#include <vector>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include "./elements/button.h"
#include "./menuTypes.h"
#include "./mouse.h"
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

    std::function<void()> m_musicStartCb{nullptr};
    std::function<void()> m_musicEndCb{nullptr};



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
        float bgImageY=0,
        const std::function<void()>& musicStartCb=nullptr,
        const std::function<void()>& musicEndCb=nullptr
    );

    ButtonMenu(const ButtonMenu&) = delete;
    ButtonMenu(ButtonMenu&&) = delete;
    ButtonMenu& operator=(const ButtonMenu&) = delete;
    ButtonMenu& operator=(ButtonMenu&&) = delete;

    ~ButtonMenu();


    MenuReturn run(SDL_Renderer* renderer, SDL_Surface* currentCanvas=nullptr);

    int runUpdate(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased);


private:
    void positionButtons(std::vector<MenuCb> btnCallbacks);

    float getMaxButtonHeight();
};



#endif