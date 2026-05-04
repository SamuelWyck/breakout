#include "./game.h"



int main() {

    Game* gamePtr {new Game{}};
    gamePtr->startGame();
    delete gamePtr;

    return 0;
};