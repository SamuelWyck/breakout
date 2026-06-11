#ifndef MY_UI_H
#define MY_UI_H


#include "../sdlUtils/userInterface/buttonMenu.h"
#include "../sdlUtils/userInterface/mouse.h"


class Ui {
public:
    ButtonMenu* mainMenu{nullptr};


    Ui(Mouse* mouse);

    Ui(const Ui&) = delete;
    Ui(Ui&&) = delete;
    Ui& operator=(const Ui&) = delete;
    Ui& operator=(Ui&&) = delete;

    ~Ui();


private:
    ButtonMenu* createMainMenu(Mouse* mouse);
};



#endif