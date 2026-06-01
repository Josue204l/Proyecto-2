//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_JUGADOR_H
#define PROYECTO_2_JUGADOR_H
#include <string>
#include <vector>
#include "Character.h"
#include "Pokemon.h"
#include "Inventario.h"

class Jugador : public Character {
private:
    std::string nombre;
    std::vector<Pokemon> equipo;
    Inventario inventario;
    int plata;
    int medallas;
public:
    Jugador();
    explicit Jugador(std::string nombre);
    std::string getNombre() override;
    int getPlata();
    int getMedallas();
    Inventario& getInventario();
    const std::vector<Pokemon>& getEquipo() const;
    bool agregarPokemon(Pokemon p);
    bool quitarPokemon(int index);
    Pokemon* getPokemon(int index);
    Pokemon* getPokemonActiv();
    bool tienePokemonVivo() const;
    int getTamano();
    void agregarPlata(int cantidad);
    bool gastarPlata(int cantidad);
    void agregarMedalla();
    void curarEquipo();
};

#endif //PROYECTO_2_JUGADOR_H