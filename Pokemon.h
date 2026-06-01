//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_POKEMON_H
#define PROYECTO_2_POKEMON_H
#include "EspeciePokemon.h"

class Pokemon {
    private:
    EspeciePokemon especie;
    int nivel;
    int experiencia;
    int hpActual;
public:
    Pokemon();
    Pokemon(const EspeciePokemon &esp,int nivel);
    EspeciePokemon getEspeciePokemon() const;
    string getNombre() const;
    TipoPokemon getTipoPokemon() const;
    int getNivel() const;
    int getExperiencia() const;
    int getHpActual() const;
    int getHPMAX()const;
    int getAtaque() const;
    int getDefensa()const ;
    bool estaMuerto() const;
    void dannioRecibido(int dannio);
    void curar(int cuanto);
    void curarFull();
    void ganarExperiencia(int cantidad);
    void subirNivel();
    bool puedeEvolucionar()const ;
    void evolucionar(const EspeciePokemon& pokemonevolucionado);
    bool operator>(const Pokemon &otro) const;
    int getNivelPoder()const;
};


#endif //PROYECTO_2_POKEMON_H