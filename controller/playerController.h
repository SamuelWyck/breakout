#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H


#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_scancode.h>
#include "./userInput.h"


class PlayerController {
    std::unordered_map<UserInput, std::string> m_codeToControlName{};
    std::unordered_map<std::string, bool> m_pressedInputs{};

    std::vector<std::string*> m_inputsToReset{};
    std::unordered_set<std::string> m_holdableInputs{};


public:
    PlayerController(
        const std::unordered_map<std::string, UserInput>& controls, 
        const std::unordered_set<std::string>& holdableControls={}
    );
    PlayerController();


    // A method to update the input linked to each control i.e. on user changing controls.
    void updateControls(const std::unordered_map<std::string, UserInput>& controls);


    // A method to get control names mapped to bools indicating if that control was triggered via user input.
    const std::unordered_map<std::string, bool>& getPressedInputs() const;


    // A method to reset all unholdable pressed inputs, this must be called at the end of the game loop.
    void resetPressedInputs();


    // A method to handle keyboard inputs from the SDL_EVENT_KEY_DOWN event.
    void handleKeyDown(SDL_Scancode keyCode);


    // A method to handle keyboard inputs from the SDL_EVENT_KEY_UP event.
    void handleKeyUp(SDL_Scancode keyCode);


    // A method to handle mouse inputs from the SDL_EVENT_MOUSE_BUTTON_DOWN event.
    void handleMouseDown(Uint8 mouseBtn);


    // A method to handle mouse inputs from the SDL_EVENT_MOUSE_BUTTON_UP event.
    void handleMouseUp(Uint8 mouseBtn);


    // A method to reset all control inputs to the false state.
    void resetAllInputs();
};



#endif