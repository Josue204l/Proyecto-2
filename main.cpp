#include <iostream>
#include <cctype>
#include "Game.h"
#include "Logger.h"

int main() {
    char respuesta = 's';

    while (respuesta == 's') {
        std::cout << "\n========================================\n";
        std::cout << " MOTOR DE AVENTURA POKEMON - ROGUELIKE\n";
        std::cout << " Cada partida ofrece una nueva aventura.\n";
        std::cout << "========================================\n";

        Game game;
        game.inicializar();
        game.correr();

        do {
            std::cout << "\n¿Quieres volver a jugar? (s/n): ";
            if (!(std::cin >> respuesta)) {
                respuesta = 'n';
                break;
            }
            respuesta = static_cast<char>(std::tolower(
                static_cast<unsigned char>(respuesta)));
            if (respuesta != 's' && respuesta != 'n') {
                std::cout << "Respuesta invalida. Escribe s o n.\n";
            }
        } while (respuesta != 's' && respuesta != 'n');
    }

    Logger::getInstancia().close();
    std::cout << "Gracias por jugar.\n";
    return 0;
}
