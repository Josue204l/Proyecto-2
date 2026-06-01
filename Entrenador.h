//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_ENTRENADOR_H
#define PROYECTO_2_ENTRENADOR_H

#include <string>
#include <vector>
#include "Pokemon.h"
class Entrenador {
    private:
    std::string nombre;
    std::vector<Pokemon> equipo;
    int plataGanada;
    public:
    Entrenador();
    Entrenador(std::string nombre, int plataGanada);
    std::string getNombre();
    int getPlataGanada();
    const std::vector<Pokemon>& getEquipo();
    void addPokemon(Pokemon &p);
    Pokemon& getPokemon(int index);
    bool tienePokemonVivos()const;
};


#endif //PROYECTO_2_ENTRENADOR_H