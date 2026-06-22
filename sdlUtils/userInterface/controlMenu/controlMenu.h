#ifndef MY_CONTROL_MENU_H
#define MY_CONTROL_MENU_H


#include <string>
#include <vector>
#include <unordered_map>
#include <SDL3/SDL_render.h>
#include "../../../controller/userInput.h"
#include "./controlInput.h"
#include "../elements/iMenuElement.h"
#include "../elements/button.h"
#include "../scrollView/scrollView.h"
#include "../../font.h"
#include "../../color.h"


class ControlMenu {
    std::unordered_map<std::string, UserInput> m_controls{};

    float m_eleGap {};

    Font* m_font {nullptr};

    Color m_color{};
    Color m_hvrColor{};

    SDL_Texture* m_bgImage {nullptr};

    std::vector<ControlInput*> m_inputs{};


public:
    ControlMenu(
        const std::unordered_map<std::string, UserInput>& controls, 
        float eleGap,
        float inputGap,
        Font* font, 
        const Color& color, 
        const Color& hvrColor,
        SDL_Texture* bgImage,
        SDL_Renderer* renderer
    ) 
        : m_controls{controls}, m_eleGap{eleGap}, m_font{font}, m_color{color}, m_hvrColor{hvrColor}, m_bgImage{bgImage} 
    {   
        std::vector<IMenuElement*> scrollElements{};
        
        constexpr float x {0.0f};
        constexpr float y {x};
        constexpr float titleWidth {x};
        float maxTitleWidth {};
        for (auto& [controlName, control] : m_controls) {
            ControlInput* input {new ControlInput{
                x, y, inputGap, titleWidth, 
                controlName, control.getInputName(),
                m_font, m_color, m_hvrColor, renderer
            }};

            if (input->getTitleDisplayWidth() > maxTitleWidth) {
                maxTitleWidth = input->getTitleDisplayWidth();
            }
            
            scrollElements.push_back(input);
            m_inputs.push_back(input);
        }
    };



private:
    
};



#endif