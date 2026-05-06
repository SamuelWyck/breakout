#ifndef USER_INPUT_WRAPPER_H
#define USER_INPUT_WRAPPER_H


#include <string>
#include <iostream>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_scancode.h>


class UserInput {
    bool m_isMouseBtn {false};
    int m_inputCode {};


public:
    explicit UserInput(Uint8 mouseBtnCode);
    explicit UserInput(SDL_Scancode keyboardBtnCode);

    std::string getInputName() const;

    bool operator==(const UserInput& input) const;
    bool operator==(SDL_Scancode scancode) const;
    bool operator==(Uint8 mouseBtn) const;

    friend std::ostream& operator<<(std::ostream& out, const UserInput& input);
};



#endif