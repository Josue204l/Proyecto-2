//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_PELEA_H
#define PROYECTO_2_PELEA_H
#include  "Jugador.h"
#include "Entrenador.h"
#include "Pokemon.h"
#include "Enums.h"
class Pelea {
private:
    static double getMultiplicadorTipo(TipoPokemon atacante, TipoPokemon defensor );
    static int calcularDannio(const Pokemon& atacante,const Pokemon& defensor);
    public:
    static bool empezarBatallaSalvaje(Jugador& juga, Pokemon pokemonSalvaje);
    static bool empezarBatallaEntrenador(Jugador& juga, Entrenador entrenador);
    static int seleccionarPokemon(Jugador& juga );
};


#endif //PROYECTO_2_PELEA_H