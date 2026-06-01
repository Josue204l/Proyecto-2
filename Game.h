//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_GAME_H
#define PROYECTO_2_GAME_H
#include "FileLoader.h"
#include "Jugador.h"
#include "World.h"


class Game {
    private:
    Jugador jugador;
    World world;
    FileLoader loader;
    int ubicacionActua;
    bool muerto;
    void elegirStarter();
    void procesarLugar();
    void revisarVictoria();
    void revisarDerrota();
    void generarReporteFinal();
    public:
    Game();
    void inicializar();
    void correr();

};


#endif //PROYECTO_2_GAME_H