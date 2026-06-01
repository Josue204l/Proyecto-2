#ifndef PROYECTO_2_ESPECIEPOKEMON_H
#define PROYECTO_2_ESPECIEPOKEMON_H
#include <string>
#include "Enums.h"
using namespace std;

// Guarda los datos base de una especie de pokemon (como una "plantilla")
// Cada pokemon individual usa una EspeciePokemon para saber sus stats
class EspeciePokemon {
private:
    string nombre;
    TipoPokemon tipo;
    int hpBase;
    int ataqueBase;
    int defensaBase;
    int nivelEvolucion;   // nivel al que evoluciona (0 si no evoluciona)
    string nombreEvolucion; // nombre de la evolucion (NADA si no evoluciona)
    Rareza rareza;

public:
    EspeciePokemon();
    EspeciePokemon(const string& nombre, TipoPokemon tipo, int hpBase, int ataqueBase,
                   int defensaBase, int nivelEvolucion, const string& nombreEvolucion, Rareza rareza);

    string getNombre() const;
    TipoPokemon getTipo() const;
    int getHpBase() const;
    int getAtaqueBase() const;
    int getDefenseBase() const;
    int getNivelEvolucion() const;
    string getNombreEvolucion() const;
    Rareza getRareza() const;
};

#endif // PROYECTO_2_ESPECIEPOKEMON_H
