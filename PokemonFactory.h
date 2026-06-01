//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_POKEMONFACTORY_H
#define PROYECTO_2_POKEMONFACTORY_H

#include <vector>
#include "Pokemon.h"
#include "EspeciePokemon.h"
class PokemonFactory {
    public:
    static Pokemon crearPoke(const EspeciePokemon& especies,int nivel);
    static Pokemon crearPokeRandom(const std :: vector<EspeciePokemon>& listaEspecies,int nivel);
};


#endif //PROYECTO_2_POKEMONFACTORY_H