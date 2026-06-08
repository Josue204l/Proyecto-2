//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_PELEA_H
#define PROYECTO_2_PELEA_H
#include  "Player.h"
#include "Trainer.h"
#include "Pokemon.h"
#include "Enums.h"
#include "BattleStrategy.h"
class Battle {
private:
    static double getMultiplicadorTipo(TipoPokemon atacante, TipoPokemon defensor );
    static int calcularDannio(const Pokemon& atacante,const Pokemon& defensor);
    static int seleccionarPokemon(Player& juga, int indiceActual);
    public:
    static bool empezarBatallaSalvaje(Player& juga, Pokemon pokemonSalvaje);
    static bool empezarBatallaEntrenador(Player& juga, Trainer entrenador,
                                         BattleStrategy& estrategia);
};


#endif //PROYECTO_2_PELEA_H
