#ifndef PROYECTO_2_JUGADOR_H
#define PROYECTO_2_JUGADOR_H
#include <string>
#include <vector>
#include "Character.h"
#include "Pokemon.h"
#include "Inventario.h"

// El jugador principal de la simulacion, hereda de Character
// Tiene un equipo de hasta 6 pokemon, un inventario y monedas
class Jugador : public Character {
private:
    string nombre;
    vector<Pokemon> equipo;  // maximo 6 pokemon
    Inventario inventario;
    int plata;     // monedas del jugador
    int medallas;  // medallas de gimnasio ganadas
public:
    Jugador();
    Jugador(string nombre);

    string getNombre() override;
    int getPlata();
    int getMedallas();
    Inventario& getInventario();
    const vector<Pokemon>& getEquipo() const;

    // Manejo del equipo
    bool agregarPokemon(Pokemon p);
    bool quitarPokemon(int indice);
    Pokemon* getPokemon(int indice);
    Pokemon* getPokemonActiv(); // retorna el primer pokemon vivo

    bool tienePokemonVivo() const;
    int getTamano();

    // Manejo de monedas
    void agregarPlata(int cantidad);
    bool gastarPlata(int cantidad);

    void agregarMedalla();
    void curarEquipo(); // cura el equipo completo
    bool equipoLleno() const;
};

#endif // PROYECTO_2_JUGADOR_H
