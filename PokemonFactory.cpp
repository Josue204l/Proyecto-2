//
// Created by Gabri on 5/31/26.
//

#include "PokemonFactory.h"
#include <cstdlib>
#include <vector>

Pokemon PokemonFactory::crearPoke(const PokemonSpecies& especie, int nivel) {
    return Pokemon(especie, nivel);
}

// Filtra especies según rareza permitida por zona (nivel bajo = solo COMMON, luego RARE, luego EPIC/LEGENDARY)
Pokemon PokemonFactory::crearPokeRandom(const std::vector<PokemonSpecies>& listaEspecies, int nivel) {
    // Determina rareza máxima permitida según nivel de zona
    Rareza maxRareza;
    if (nivel <= 5)       maxRareza = Rareza::COMMON;
    else if (nivel <= 10) maxRareza = Rareza::RARE;
    else if (nivel <= 15) maxRareza = Rareza::EPIC;
    else                  maxRareza = Rareza::LEGENDARY;

    // Filtra especies que no superen la rareza permitida
    std::vector<const PokemonSpecies*> candidatos;
    for (const auto& e : listaEspecies) {
        if ((int)e.getRareza() <= (int)maxRareza)
            candidatos.push_back(&e);
    }

    // Si no hay candidatos (no debería pasar), usa cualquiera
    if (candidatos.empty())
        return Pokemon(listaEspecies[rand() % (int)listaEspecies.size()], nivel);

    int idx = rand() % (int)candidatos.size();
    return Pokemon(*candidatos[idx], nivel);
}
