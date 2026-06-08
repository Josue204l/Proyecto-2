//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_GAME_H
#define PROYECTO_2_GAME_H
#include <vector>
#include "FileLoader.h"
#include "Player.h"
#include "World.h"
#include "Objective.h"

class Game {
private:
    Player jugador;
    World world;
    FileLoader loader;
    int ubicacionActual;
    bool muerto;
    bool inicializado;
    bool ligaGanada;
    int evolucionesRealizadas;
    std::vector<Objective> objetivos;
    void elegirStarter();
    void procesarLugar(Location* lugar);
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
