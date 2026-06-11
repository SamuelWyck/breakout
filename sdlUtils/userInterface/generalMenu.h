#ifndef MY_GENERAL_MENU_H
#define MY_GENERAL_MENU_H


#include <vector>
#include <unordered_map>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include "./elements/iMenuElement.h"
#include "./elements/button.h"
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
    );

    GeneralMenu(const GeneralMenu&) = delete;
    GeneralMenu(GeneralMenu&&) = delete;
    GeneralMenu& operator=(const GeneralMenu&) = delete;
    GeneralMenu& operator=(GeneralMenu&&) = delete;

    ~GeneralMenu();


    MenuReturn run(SDL_Renderer* renderer, SDL_Surface* currentCanvas=nullptr);

    int runUpdate(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased);


private:
    void positionElements(
        const std::vector<IMenuElement*>& elements, 
        const std::vector<MenuCb>& btnCallbacks,
        float eleCenterX, float eleStartY, float eleGap
    );
};



#endif