#ifndef MY_UI_H
#define MY_UI_H


#include "../sdlUtils/userInterface/buttonMenu.h"
#include "../sdlUtils/userInterface/mouse.h"
#include "../sdlUtils/userInterface/generalMenu.h"
#include "../sdlUtils/audio/audioManager.h"


class Ui {
public:
    ButtonMenu* mainMenu{nullptr};
    ButtonMenu* settingsMenu{nullptr};
    GeneralMenu* audioMenu{nullptr};


    Ui(Mouse* mouse, AudioManager* audio);

    Ui(const Ui&) = delete;
    Ui(Ui&&) = delete;
    Ui& operator=(const Ui&) = delete;
    Ui& operator=(Ui&&) = delete;

    ~Ui();


private:
    ButtonMenu* createMainMenu(Mouse* mouse);

    ButtonMenu* createSettingsMenu(Mouse* mouse);

    GeneralMenu* createAudioMenu(Mouse* mouse, AudioManager* audio);
};



#endif