#ifndef MY_CONTROL_INPUT_H
#define MY_CONTROL_INPUT_H


#include <string>
#include <utility>
#include <string_view>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include "../../color.h"
#include "../../font.h"
#include "../elements/button.h"
#include "../elements/iMenuElement.h"
#include "../elements/textDisplay.h"


class ControlInput : public IMenuElement {
    float m_gap {};
    float m_maxTitleWidth {};

    SDL_Texture* m_btnImg {nullptr};
    SDL_Texture* m_btnHvrImg {nullptr};
    SDL_Surface* m_backupSurface {nullptr};
    
    Font* m_font {nullptr};

    Button* m_inputBtn {nullptr};
    TextDisplay m_title{};

    Color m_color{};
    Color m_hvrColor{};

    float m_centerX {};
    float m_centerY {};
    bool m_useCenter {false};

    std::string m_inputName{};


public:
    ControlInput(
        float x, 
        float y, 
        float gap, 
        float maxTitleWidth, 
        std::string_view title, 
        std::string_view inputName,
        Font* font,
        const Color& color,
        const Color& hvrColor,
        SDL_Renderer* renderer
    );

    ControlInput(const ControlInput& other);

    ControlInput(ControlInput&&) = delete;
    ControlInput& operator=(ControlInput&) = delete;
    ControlInput& operator=(ControlInput&&) = delete;
    
    ~ControlInput();


    void update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased);

    
    bool clicked() const;


    void unclick();


    void updateInputBtn(SDL_Renderer* renderer, std::string_view inputName);


    void setMaxTitleWidth(float maxTitleWidth);
    


    // interface methods

    void update(SDL_Renderer*) override;

    std::pair<float, float> topleft() const override;
    void setTopleft(float x, float y) override;

    std::pair<float, float> center() const override;
    void setCenter(float x, float y) override;

    void setX(float x) override;
    void setY(float y) override;

    float width() const override;
    float height() const override;


private:
    void destroyBtnImages();

    void createInputBtnImgs(SDL_Renderer* renderer, std::string_view inputName);

    void positionBtn();
};



#endif