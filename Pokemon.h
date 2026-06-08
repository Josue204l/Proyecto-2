#ifndef PROYECTO_2_POKEMON_H
#define PROYECTO_2_POKEMON_H
#include "EspeciePokemon.h"

// Representa un pokemon individual con su nivel, experiencia y HP actual
// Usa EspeciePokemon para obtener los stats base
class Pokemon {
private:
    EspeciePokemon especie;
    int nivel;
    int experiencia;
    int hpActual;

public:
    Pokemon();
    Pokemon(const EspeciePokemon& especie, int nivel);

    // Getters de informacion basica
    EspeciePokemon getEspeciePokemon() const;
    string getNombre() const;
    TipoPokemon getTipoPokemon() const;
    int getNivel() const;
    int getExperiencia() const;

    // Stats calculados segun nivel
    int getHpActual() const;
    int getHPMAX() const;
    int getAtaque() const;
    int getDefensa() const;

    // Estado del pokemon
    bool estaMuerto() const;

    // Acciones en combate
    void dannioRecibido(int dannio);
    void curar(int cantidad);
    void curarFull();

    // Subir nivel y evolucionar
    void ganarExperiencia(int cantidad);
    void subirNivelesDirecto(int n);
    void subirNivel();
    bool puedeEvolucionar() const;
    void evolucionar(const EspeciePokemon& nuevaEspecie);

    // Operador > para comparar poder entre dos pokemon
    bool operator>(const Pokemon& otro) const;
    int getNivelPoder() const;
};

#endif // PROYECTO_2_POKEMON_H
