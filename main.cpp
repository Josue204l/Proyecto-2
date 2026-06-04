#include <iostream>
#include <windows.h>
#include "Game.h"

int main() {
    SetConsoleOutputCP(65001);  // UTF-8
    SetConsoleCP(65001);
    Game game;
    game.inicializar();
    game.correr();
    return 0;
}
