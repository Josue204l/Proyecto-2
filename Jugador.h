//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_JUGADOR_H
#define PROYECTO_2_JUGADOR_H
#include <string>
#include <vector>
#include "Pokemon.h"
#include "Inventario.h"
class Jugador {
    private:
    std::string nombre;
    std::vector<Pokemon> equipo;
    Inventario inventario;
    int plata;
    int medallas;
    public:
    Jugador();
    Jugador(std::string nombre);
    std::string getNombre();
    int getPlata();
    int getMedallas();
    Inventario &getInventario();
    const std::vector<Pokemon>& getEquipo()const;
    bool agregarPokemon(Pokemon p);
    bool quitarPokemon(Pokemon p);
    Pokemon* getPokemon(int index);
    Pokemon *getPokemonActiv();
    bool tienePokemonVivo() const;
    int getTamano();
    void agregarPlata(int cantidad);
    bool gastarPlata(int cantidad);
    void agregarMedalla();
    void curarEquipo();

};


#endif //PROYECTO_2_JUGADOR_H