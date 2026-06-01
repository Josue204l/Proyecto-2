//
// Created by Gabri on 5/31/26.
//

#include "PokemonFactory.h"
#include <cstdlib>

Pokemon PokemonFactory::crearPoke(const EspeciePokemon& especie, int nivel) {
    return Pokemon(especie, nivel);
}

Pokemon PokemonFactory::crearPokeRandom(const std::vector<EspeciePokemon>& listaEspecies, int nivel) {
    int idx = rand() % (int)listaEspecies.size();
    return Pokemon(listaEspecies[idx], nivel);
}