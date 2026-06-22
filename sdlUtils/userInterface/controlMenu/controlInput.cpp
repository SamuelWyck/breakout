#include <string>
#include <utility>
#include <string_view>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_rect.h>
#include "../../color.h"
#include "../../font.h"
#include "./controlInput.h"



ControlInput::ControlInput(
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
) 
    : m_gap{gap},
    m_maxTitleWidth{maxTitleWidth}, 
    m_font{font}, 
    m_title{x, y, title, font, color}, 
    m_color{color},
    m_hvrColor{hvrColor},
    m_titleText{title}
{
    updateInputBtn(renderer, inputName);
    positionBtn();
};

ControlInput::ControlInput(const ControlInput& other) 
    : m_gap{other.m_gap},
    m_maxTitleWidth{other.m_maxTitleWidth},
    m_font{other.m_font},
    m_title{other.m_title},
    m_color{other.m_color},
    m_hvrColor{other.m_hvrColor},
    m_inputName{other.m_inputName},
    m_titleText{other.m_titleText}
{
    m_backupSurface = m_font->renderSurface(m_inputName, m_color.getSDLColor());
};

ControlInput::~ControlInput() {
    destroyBtnImages();
    delete m_inputBtn;
};


void ControlInput::update(SDL_Renderer* renderer, const SDL_FPoint& mousePos, bool mousePressed, bool mouseReleased) {
    m_title.update(renderer);

    if (!m_inputBtn) {
        updateInputBtn(renderer, m_inputName);
        positionBtn();
    }
    m_inputBtn->update(renderer, mousePos, mousePressed, mouseReleased);
};


bool ControlInput::clicked() const {
    if (m_inputBtn) {
        return m_inputBtn->clicked();
    }
    return false;
};


void ControlInput::unclick() {
    if (m_inputBtn) {
        m_inputBtn->unclick();
    }
};


void ControlInput::updateInputBtn(SDL_Renderer* renderer, std::string_view inputName) {
    float btnX {0.0f};
    float btnY {0.0f};
    if (m_inputBtn) {
        auto [x, y] {m_inputBtn->topleft()};
        btnX = x;
        btnY = y;
    }
    
    createInputBtnImgs(renderer, inputName);
    delete m_inputBtn;
    m_inputBtn = new Button{btnX, btnY, m_btnImg, m_btnHvrImg};

    if (m_useCenter) {
        setCenter(m_centerX, m_centerY);
    }
};


void ControlInput::setMaxTitleWidth(float maxTitleWidth) {
    m_maxTitleWidth = maxTitleWidth;
};


const std::string& ControlInput::title() const {
    return m_titleText;
};


float ControlInput::getTitleDisplayWidth() const {
    return m_title.width();
};



// interface methods

void ControlInput::update(SDL_Renderer*) {
};

std::pair<float, float> ControlInput::topleft() const {
    return m_title.topleft();
};

void ControlInput::setTopleft(float x, float y) {
    m_title.setTopleft(x, y);

    if (m_inputBtn) {
        float btnX {x + titleWidth() + m_gap};
        m_inputBtn->setTopleft(btnX, y);
    }

    m_useCenter = false;
};

std::pair<float, float> ControlInput::center() const {
    float btnWidth {(m_inputBtn) ? m_inputBtn->width() : static_cast<float>(m_backupSurface->w)};
    float totalWidth {titleWidth() + m_gap + btnWidth};
    float totalHeight {m_title.height()};

    auto [x, y] {m_title.topleft()};
    return {x + (totalWidth / 2.0f), y + (totalHeight / 2.0f)};
};

void ControlInput::setCenter(float x, float y) {
    m_centerX = x;
    m_centerY = y;
    m_useCenter = true;

    float btnWidth {(m_inputBtn) ? m_inputBtn->width() : static_cast<float>(m_backupSurface->w)};
    float totalWidth {titleWidth() + m_gap + btnWidth};
    float totalHeight {m_title.height()};

    float titleX {x - (totalWidth / 2.0f)};
    float titleY {y - (totalHeight / 2.0f)};
    m_title.setTopleft(titleX, titleY);
    if (m_inputBtn) {
        m_inputBtn->setTopleft(titleX + titleWidth() + m_gap, titleY);
    }
};

void ControlInput::setX(float x) {
    m_useCenter = false;

    m_title.setX(x);
    if (m_inputBtn) {
        m_inputBtn->setX(x + m_maxTitleWidth + m_gap);
    }
};

void ControlInput::setY(float y) {
    m_useCenter = false;

    m_title.setY(y);
    if (m_inputBtn) {
        m_inputBtn->setY(y);
    }
};

float ControlInput::width() const {
    float btnWidth {(m_inputBtn) ? m_inputBtn->width() : static_cast<float>(m_backupSurface->w)};
    return titleWidth() + m_gap + btnWidth;
};

float ControlInput::height() const {
    return m_title.height();
};



void ControlInput::destroyBtnImages() {
    SDL_DestroyTexture(m_btnImg);
    SDL_DestroyTexture(m_btnHvrImg);
    SDL_DestroySurface(m_backupSurface);
};

void ControlInput::createInputBtnImgs(SDL_Renderer* renderer, std::string_view inputName) {
    destroyBtnImages();
    m_btnImg = m_font->renderTexture(renderer, inputName, m_color.getSDLColor());
    m_btnHvrImg = m_font->renderTexture(renderer, inputName, m_hvrColor.getSDLColor());
    m_inputName = inputName;
};

void ControlInput::positionBtn() {
    auto [titleX, titleY] {m_title.topleft()};
    float btnX {titleX + m_maxTitleWidth + m_gap};
    m_inputBtn->setTopleft(btnX, titleY);
};

float ControlInput::titleWidth() const {
    return (m_maxTitleWidth > m_title.width()) ? m_maxTitleWidth : m_title.width();
};