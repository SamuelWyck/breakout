#ifndef MY_CONTROL_MENU_H
#define MY_CONTROL_MENU_H


#include <string>
#include <vector>
#include <unordered_set>
#include <functional>
#include <unordered_map>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_events.h>
#include "../../playerController/userInput.h"
#include "./controlInput.h"
#include "../elements/iMenuElement.h"
#include "../elements/button.h"
#include "../../collision/fRect.h"
#include "../elements/textDisplay.h"
#include "../scrollView/scrollView.h"
#include "../../font.h"
#include "../../color.h"
#include "../mouse.h"
#include "../menuTypes.h"


class ControlMenu {
    std::unordered_map<std::string, UserInput> m_controls{};
    std::vector<ControlInput*> m_inputs{};

    Mouse* m_mouse {nullptr};

    std::function<void(const std::unordered_map<std::string, UserInput>&)> m_saveCb{};

    Font* m_font {nullptr};
    Color m_color{};
    Color m_hvrColor{};

    SDL_Texture* m_bgImage {nullptr};
    ScrollView* m_scrollView {nullptr};

    // menu banner
    TextDisplay m_instructionBanner{};

    // exit btn variables
    SDL_Texture* m_exitBtnImg {nullptr};
    SDL_Texture* m_exitBtnHvrImg {nullptr};
    Button* m_exitBtn {nullptr};

    // general popup variables 
    float m_popupPadding {20};
    float m_popupBorderWidth {8};

    // editing popup variables
    TextDisplay m_editingMsg{};
    FRect m_editingBorderRect{};
    FRect m_editingBgImgRect{};
    FRect m_editingBgImgClipArea{};

    // error popup variables
    TextDisplay m_errorMsg{};
    FRect m_errorBgImgRect{};
    FRect m_errorBgImgClipArea{};
    FRect m_errorBorderRect{};



public:
    ControlMenu(
        const std::unordered_map<std::string, UserInput>& controls, 
        Mouse* mouse,
        const std::function<void(const std::unordered_map<std::string, UserInput>&)>& saveCb,
        float scrollviewHeight,
        float eleGap,
        float inputGap,
        float scrollGap,
        float scrollWidth,
        Font* font, 
        const Color& color, 
        const Color& hvrColor,
        SDL_Texture* bgImage,
        SDL_Renderer* renderer,
        const Color& scrollColor,
        const Color& scrollBgColor,
        SDL_Texture* scrollImg=nullptr,
        SDL_Texture* scrollBgImg=nullptr
    );

    ControlMenu(const ControlMenu&) = delete;
    ControlMenu(ControlMenu&&) = delete;
    ControlMenu& operator=(const ControlMenu&) = delete;
    ControlMenu& operator=(ControlMenu&&) = delete;
    
    ~ControlMenu();


    MenuReturn run(SDL_Renderer* renderer, SDL_Surface*);



private:
    bool noDuplicates();


    void updateControls(SDL_Renderer* renderer, const UserInput& newControl, ControlInput* inputToChange);


    std::pair<float, float> calcMaxTitleAndInputWidth();


    void setMaxTitleWidth(float titleWidth);


    void createErrorPopup();


    void drawErrorPopup(SDL_Renderer* renderer);


    void createExitBtn(SDL_Renderer* renderer);


    void createEditingPopup();
    

    void drawEditingPopup(SDL_Renderer* renderer);


    void positionBanner();


    void createScrollView(
        float height, 
        float inputGap, 
        float eleGap, 
        float scrollGap,
        float scrollWidth,
        const Color& scrollColor,
        const Color& scrollBgColor,
        SDL_Texture* scrollImg,
        SDL_Texture* scrollBgImg,
        SDL_Renderer* renderer
    );
};



#endif