//
// Created by Gabri on 5/31/26.
//

#include "Pokemon.h"

Pokemon::Pokemon(): nivel(1), experiencia(0),hpActual(1){
}

Pokemon::Pokemon(const EspeciePokemon &esp, int nivel) : especie(esp ),nivel(nivel),experiencia(0){
    hpActual = getHPMAX();
}

std::string Pokemon::getNombre() const {
    return especie.getNombre();
}
TipoPokemon Pokemon::getTipoPokemon() const {
    return especie.getTipo();
}
int Pokemon::getNivel() const {
    return nivel;
}
int Pokemon::getExperiencia() const {
    return experiencia;
}
int Pokemon::getHpActual() const {
    return hpActual;
}

int Pokemon::getHPMAX() const {
    return especie.getHpBase() + (nivel * 3);
}

int Pokemon::getAtaque() const {
    return especie.getAtaqueBase() + (nivel * 2);
}
int Pokemon::getDefensa() const {
    return especie.getDefenseBase() + (nivel * 2);
}
bool Pokemon::estaMuerto() const {
    return hpActual <= 0;
}

void Pokemon::dannioRecibido(int dannio) {
    hpActual -= dannio;
    if (hpActual < 0) {
        hpActual = 0;
    }
}

void Pokemon::curar(int cuanto) {
    hpActual += cuanto;
    if (hpActual > getHPMAX() ) {
        hpActual = getHPMAX();
    }
}

void Pokemon::curarFull() {
    hpActual = getHPMAX();
}

void Pokemon::ganarExperiencia(int cantidad) {
    experiencia += cantidad;
    while (experiencia >= nivel * 100) {
        experiencia -= nivel * 100;
        subirNivel();
    }
}
void Pokemon::subirNivel() {
    nivel++;
    hpActual = getHPMAX();
}
bool  Pokemon::puedeEvolucionar() const {
    if (especie.getNombreEvolucion() == "NADA") {
        return false;
    }
    return nivel >= especie.getNivelEvolucion();
}
void Pokemon::evolucionar(const EspeciePokemon& evolucion) {
    especie = evolucion;
    hpActual = getHPMAX();
}

int Pokemon::getNivelPoder() const {
    return getAtaque() + getDefensa() + getHPMAX();
}

bool Pokemon::operator>(const Pokemon &otro) const {
    return getNivelPoder() > otro.getNivelPoder();
}
