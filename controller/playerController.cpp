#include <unordered_map>
#include <string>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_scancode.h>
#include "./playerController.h"
#include "./userInput.h"



PlayerController::PlayerController(
    const std::unordered_map<std::string, UserInput>& controls, 
    const std::unordered_set<std::string>& holdableControls
) : m_holdableInputs{holdableControls} {
    updateControls(controls);
};

PlayerController::PlayerController() {
};


void PlayerController::updateControls(const std::unordered_map<std::string, UserInput>& controls) {
    m_codeToControlName.clear();
    m_pressedInputs.clear();
    m_inputsToReset.clear();

    for (auto& entry : controls) {
        const std::string& controlName {entry.first};
        const UserInput& controlCode{entry.second};

        m_pressedInputs[controlName] = false;
        m_codeToControlName[controlCode] = controlName;
    }
};


const std::unordered_map<std::string, bool>& PlayerController::getPressedInputs() const {
    return m_pressedInputs;
};


void PlayerController::resetPressedInputs() {
    for (const std::string* controlName : m_inputsToReset) {
        m_pressedInputs[*controlName] = false;
    }

    m_inputsToReset.clear();
};


void PlayerController::handleKeyDown(SDL_Scancode keyCode) {
    UserInput inputCode{keyCode};
    if (!m_codeToControlName.contains(inputCode)) {
        return;
    }

    std::string& controlName{m_codeToControlName[inputCode]};
    m_pressedInputs[controlName] = true;

    if (!m_holdableInputs.contains(controlName)) {
        m_inputsToReset.push_back(&controlName);
    }
};


void PlayerController::handleKeyUp(SDL_Scancode keyCode) {
    UserInput inputCode{keyCode};
    if (!m_codeToControlName.contains(inputCode)) {
        return;
    }

    std::string& controlName{m_codeToControlName[inputCode]};
    m_pressedInputs[controlName] = false;
};


void PlayerController::handleMouseDown(Uint8 mouseBtn) {
    UserInput inputCode{mouseBtn};
    if (!m_codeToControlName.contains(inputCode)) {
        return;
    }

    std::string& controlName{m_codeToControlName[inputCode]};
    m_pressedInputs[controlName] = true;

    if (!m_holdableInputs.contains(controlName)) {
        m_inputsToReset.push_back(&controlName);
    }
};


void PlayerController::handleMouseUp(Uint8 mouseBtn) {
    UserInput inputCode{mouseBtn};
    if (!m_codeToControlName.contains(inputCode)) {
        return;
    }

    std::string& controlName{m_codeToControlName[inputCode]};
    m_pressedInputs[controlName] = false;
};


void PlayerController::resetAllInputs() {
    m_inputsToReset.clear();

    for (auto& entry : m_pressedInputs) {
        const std::string& controlName{entry.first};
        m_pressedInputs[controlName] = false;
    }
};