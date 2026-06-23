#include <string>
#include <iostream>
#include <functional>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>
#include "./userInput.h"



UserInput::UserInput() 
    : m_isMouseBtn{false}, m_inputCode{0} {
};

UserInput::UserInput(Uint8 mouseBtnCode) 
    : m_isMouseBtn{true}, m_inputCode{mouseBtnCode} {
};

UserInput::UserInput(SDL_Scancode keyboardBtnCode) 
    : m_isMouseBtn{false}, m_inputCode{keyboardBtnCode} {
};


std::string UserInput::getInputName() const {
    if (m_isMouseBtn) {
        return std::string{'M' + std::to_string(m_inputCode)};
    }
    return std::string{SDL_GetScancodeName(static_cast<SDL_Scancode>(m_inputCode))};
};

bool UserInput::isMouseInput() const {
    return m_isMouseBtn;
};

int UserInput::inputCode() const {
    return m_inputCode;
};


bool UserInput::operator==(const UserInput& input) const {
    return input.m_isMouseBtn == m_isMouseBtn && m_inputCode == input.m_inputCode;
};

bool UserInput::operator==(SDL_Scancode scancode) const {
    if (m_isMouseBtn) {
        return false;
    }

    return static_cast<SDL_Scancode>(m_inputCode) == scancode;
};

bool UserInput::operator==(Uint8 mouseBtn) const {
    if (!m_isMouseBtn) {
        return false;
    }

    return mouseBtn == m_inputCode;
};


std::ostream& operator<<(std::ostream& out, const UserInput& input) {
    if (input.m_isMouseBtn) {
        out << 'M' << input.m_inputCode;
        return out;
    }

    out << SDL_GetScancodeName(static_cast<SDL_Scancode>(input.m_inputCode));
    return out;
};