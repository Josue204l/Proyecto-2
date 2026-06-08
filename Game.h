//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_GAME_H
#define PROYECTO_2_GAME_H
#include <vector>
#include "FileLoader.h"
#include "Jugador.h"
#include "World.h"
#include "Objective.h"

class Game {
private:
    Jugador jugador;
    World world;
    FileLoader loader;
    int ubicacionActual;
    bool muerto;
    std::vector<Objective> objetivos;
    void elegirStarter();
    void procesarLugar(Lugar* lugar);
    void revisarVictoria();
    void revisarDerrota();
    void generarReporteFinal();
    void revisarEvoluciones();
public:
    Game();
    void inicializar();
    void correr();
};

#endif //PROYECTO_2_GAME_H