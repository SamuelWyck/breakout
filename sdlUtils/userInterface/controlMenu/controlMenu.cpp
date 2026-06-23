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
#include "./controlMenu.h"



ControlMenu::ControlMenu(
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
    SDL_Texture* scrollImg,
    SDL_Texture* scrollBgImg
) 
    : m_controls{controls}, 
    m_mouse{mouse}, 
    m_saveCb{saveCb},
    m_font{font}, 
    m_color{color}, 
    m_hvrColor{hvrColor}, 
    m_bgImage{bgImage},
    m_instructionBanner{0, 0, "SELECT A KEYBIND TO EDIT", font, color},
    m_editingMsg{0, 0, "ENTER INPUT\nESC TO CANCEL", font, color},
    m_errorMsg{0, 0, "DUPLICATE\nKEYBINDS\n\nCONTINUE", font, color}
{   
    createScrollView(
        scrollviewHeight, 
        inputGap, 
        eleGap, 
        scrollGap,
        scrollWidth,
        scrollColor, 
        scrollBgColor, 
        scrollImg, 
        scrollBgImg, 
        renderer
    );

    positionBanner();
    createExitBtn(renderer);
    createEditingPopup();
    createErrorPopup();
};

ControlMenu::~ControlMenu() {
    delete m_scrollView;

    for (ControlInput* ele : m_inputs) {
        delete ele;
    }

    delete m_exitBtn;
    SDL_DestroyTexture(m_exitBtnImg);
    SDL_DestroyTexture(m_exitBtnHvrImg);
};


MenuReturn ControlMenu::run(SDL_Renderer* renderer, SDL_Surface*) {

    bool running {true};
    bool forceExit {false};
    MenuReturn returnVal{};

    ControlInput* inputToEdit {nullptr};
    bool editingControl {false};
    bool duplicatesError {false};
    bool controlsChanged {false};

    while (running) {
        bool mousePressed {false};
        bool mouseReleased {false};

        SDL_Event event{};
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
                forceExit = true;
                returnVal = {};
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                m_mouse->update(event.motion.xrel, event.motion.yrel);
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseReleased = true;
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (editingControl) {
                    updateControls(renderer, UserInput{event.button.button}, inputToEdit);
                    editingControl = false;
                    controlsChanged = true;

                } else if (event.button.button == SDL_BUTTON_LEFT) {
                    if (duplicatesError) {
                        duplicatesError = false;
                    } else {
                        mousePressed = true;
                    }
                }
            } else if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                if (editingControl && event.key.scancode != SDL_SCANCODE_ESCAPE) {
                    updateControls(renderer, UserInput{event.key.scancode}, inputToEdit);
                    editingControl = false;
                    controlsChanged = true;

                } else if (editingControl && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    editingControl = false;
                }
            }
        }


        if (m_exitBtn->clicked()) {
            m_exitBtn->unclick();
            if (noDuplicates()) {
                running = false;
                returnVal = {{0, -1}};
            } else {
                duplicatesError = true;
            }
        }


        for (ControlInput* input : m_inputs) {
            if (input->clicked()) {
                input->unclick();
                if (!editingControl && !duplicatesError) {
                    inputToEdit = input;
                    editingControl = true;
                }
            }
        }


        SDL_FPoint mousePos{m_mouse->getPos()};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, m_bgImage, nullptr, nullptr);
        m_instructionBanner.update(renderer);
        m_exitBtn->update(renderer, mousePos, mousePressed, mouseReleased);

        m_scrollView->update(renderer, mousePos, mousePressed, mouseReleased);
        
        if (editingControl) {
            drawEditingPopup(renderer);
        } else if (duplicatesError) {
            drawErrorPopup(renderer);
        }
        
        m_mouse->draw(renderer);
        SDL_RenderPresent(renderer);
    }

    constexpr float scrollViewTop {0};
    m_scrollView->setScrollPos(scrollViewTop);
    if (!forceExit && m_saveCb && controlsChanged) {
        m_saveCb(m_controls);
    }
    return returnVal;
};



bool ControlMenu::noDuplicates() {
    std::unordered_set<UserInput> seenControls{};
    for (auto& entry : m_controls) {
        UserInput& control{entry.second};
        if (seenControls.contains(control)) {
            return false;
        }
        seenControls.insert(control);
    }

    return true;
};


void ControlMenu::updateControls(SDL_Renderer* renderer, const UserInput& newControl, ControlInput* inputToChange) {
    inputToChange->updateInputBtn(renderer, newControl.getInputName());
    m_controls[inputToChange->title()] = newControl;

    auto [maxTitleWidth, maxInputWidth] {calcMaxTitleAndInputWidth()};
    setMaxTitleWidth(maxTitleWidth);

    m_scrollView->setWidth(maxInputWidth);
    auto [scrollViewX, scrollViewY] {m_scrollView->topleft()};
    float newScrollViewX {(m_bgImage->w / 2.0f) - (m_scrollView->width() / 2.0f)};
    m_scrollView->setTopleft(newScrollViewX, scrollViewY);
};


std::pair<float, float> ControlMenu::calcMaxTitleAndInputWidth() {
    float maxTitleWidth {0.0f};
    float maxInputWidth {0.0f};

    for (ControlInput* elePtr : m_inputs) {
        if (elePtr->width() > maxInputWidth) {
            maxInputWidth = elePtr->width();
        }
        if (elePtr->getTitleDisplayWidth() > maxTitleWidth) {
            maxTitleWidth = elePtr->getTitleDisplayWidth();
        }
    }

    return {maxTitleWidth, maxInputWidth};
};


void ControlMenu::setMaxTitleWidth(float titleWidth) {
    for (ControlInput* elePtr : m_inputs) {
        elePtr->setMaxTitleWidth(titleWidth);
    }
};


void ControlMenu::createErrorPopup() {
    m_errorMsg.setCenter(m_bgImage->w / 2.0f, m_bgImage->h / 2.0f);
    m_errorMsg.setWrapLength(0); // wrap on newlines

    m_errorBgImgRect.setSize(
        m_errorMsg.width() + (2 * m_popupPadding),
        m_errorMsg.height() + (2 * m_popupPadding)
    );
    m_errorBgImgRect.setCenter(m_errorMsg.center());
    m_errorBgImgClipArea.setSize(m_errorBgImgRect.size());

    m_errorBorderRect.setSize(
        m_errorBgImgRect.width() + (2 * m_popupBorderWidth),
        m_errorBgImgRect.height() + (2 * m_popupBorderWidth)
    );
    m_errorBorderRect.setCenter(m_errorBgImgRect.center());
};


void ControlMenu::drawErrorPopup(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_color.red(), m_color.green(), m_color.blue(), m_color.alpha());
    SDL_RenderFillRect(renderer, &m_errorBorderRect.getSDLFRect());

    SDL_RenderTexture(
        renderer, m_bgImage, 
        &m_errorBgImgClipArea.getSDLFRect(), 
        &m_errorBgImgRect.getSDLFRect()
    );

    m_errorMsg.update(renderer);
};


void ControlMenu::createExitBtn(SDL_Renderer* renderer) {
    m_exitBtnImg = m_font->renderTexture(renderer, "BACK", m_color.getSDLColor());
    m_exitBtnHvrImg = m_font->renderTexture(renderer, "BACK", m_hvrColor.getSDLColor());

    float btnX {m_bgImage->w * .03f};
    float btnY {m_bgImage->h - m_exitBtnImg->h - btnX}; // btn will be ~3% spaced from bottomleft corner
    m_exitBtn = new Button{btnX, btnY, m_exitBtnImg, m_exitBtnHvrImg};
};


void ControlMenu::createEditingPopup() {
    m_editingMsg.setCenter(m_bgImage->w / 2.0f, m_bgImage->h / 2.0f);
    constexpr int wrapOnNewline {0};
    m_editingMsg.setWrapLength(wrapOnNewline);

    m_editingBgImgRect.setSize(
        m_editingMsg.width() + (m_popupPadding * 2), 
        m_editingMsg.height() + (m_popupPadding * 2)
    );
    m_editingBgImgRect.setCenter(m_editingMsg.center());
    m_editingBgImgClipArea.setSize(m_editingBgImgRect.size());

    m_editingBorderRect.setSize(
        m_editingBgImgRect.width() + (m_popupBorderWidth * 2),
        m_editingBgImgRect.height() + (m_popupBorderWidth * 2)
    );
    m_editingBorderRect.setCenter(m_editingBgImgRect.center());
};


void ControlMenu::drawEditingPopup(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, m_color.red(), m_color.green(), m_color.blue(), m_color.alpha());
    SDL_RenderFillRect(renderer, &m_editingBorderRect.getSDLFRect());

    SDL_RenderTexture(
        renderer, m_bgImage, 
        &m_editingBgImgClipArea.getSDLFRect(),
        &m_editingBgImgRect.getSDLFRect()
    );
    m_editingMsg.update(renderer);
};


void ControlMenu::positionBanner() {
    float bannerHeight {m_instructionBanner.height()};
    float bannerY {m_bgImage->h * .08f}; // make banner y 8% of bg img height
    m_instructionBanner.setCenter(m_bgImage->w / 2.0f, (bannerY + (bannerHeight / 2.0f)));
};


void ControlMenu::createScrollView(
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
) {
    std::vector<IMenuElement*> scrollElements{};
    
    constexpr float x {0.0f};
    constexpr float y {x};
    constexpr float titleWidth {x};
    for (auto& [controlName, control] : m_controls) {
        ControlInput* input {new ControlInput{
            x, y, inputGap, titleWidth, 
            controlName, control.getInputName(),
            m_font, m_color, m_hvrColor, renderer
        }};

        scrollElements.push_back(input);
        m_inputs.push_back(input);
    }


    auto [maxTitleWidth, maxInputWidth] {calcMaxTitleAndInputWidth()};
    setMaxTitleWidth(maxTitleWidth);

    float scrollViewX {(m_bgImage->w / 2.0f) - (maxInputWidth / 2.0f)};
    float scrollViewY {(m_bgImage->h / 2.0f) - (height / 2.0f)};

    constexpr float scrollMinHeight {30};
    m_scrollView = new ScrollView{
        scrollViewX, 
        scrollViewY, 
        maxInputWidth, 
        height, 
        eleGap,
        scrollGap,
        scrollElements,
        scrollWidth,
        scrollMinHeight,
        scrollColor.getSDLColor(),
        scrollBgColor.getSDLColor(),
        scrollImg,
        scrollBgImg
    };
};