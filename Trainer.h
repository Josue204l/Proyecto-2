#ifndef PROYECTO_2_ENTRENADOR_H
#define PROYECTO_2_ENTRENADOR_H
#include <string>
#include <vector>
#include "Pokemon.h"

// Representa un entrenador o lider de gimnasio que el jugador puede enfrentar
class Trainer {
private:
    string nombre;
    vector<Pokemon> equipo;
    int plataGanada; // monedas que da al ser derrotado

public:
    Trainer();
    Trainer(string nombre, int plataGanada);

    string getNombre();
    int getPlataGanada();
    const vector<Pokemon>& getEquipo();
    void addPokemon(const Pokemon& p);
    Pokemon& getPokemon(int indice);
    bool tienePokemonVivos() const;
};

#endif // PROYECTO_2_ENTRENADOR_H
