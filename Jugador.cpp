#include "Jugador.h"

// Constructor por defecto, el jugador se llama Ash y empieza con 500 monedas
Jugador::Jugador() : nombre("Ash"), plata(500), medallas(0) {
}

Jugador::Jugador(string nombre) {
    this->nombre = nombre;
    this->plata = 500;
    this->medallas = 0;
}

string Jugador::getNombre() { return nombre; }
int Jugador::getPlata() { return plata; }
int Jugador::getMedallas() { return medallas; }
Inventario& Jugador::getInventario() { return inventario; }
const vector<Pokemon>& Jugador::getEquipo() const { return equipo; }

// Solo se pueden tener 6 pokemon en el equipo
bool Jugador::agregarPokemon(Pokemon p) {
    if ((int)equipo.size() >= 6) return false;
    equipo.push_back(p);
    return true;
}

bool Jugador::quitarPokemon(int indice) {
    if (indice < 0 || indice >= (int)equipo.size()) return false;
    equipo.erase(equipo.begin() + indice);
    return true;
}

Pokemon* Jugador::getPokemon(int indice) {
    if (indice < 0 || indice >= (int)equipo.size()) return nullptr;
    return &equipo[indice];
}

// Busca el primer pokemon que no este debilitado
Pokemon* Jugador::getPokemonActiv() {
    for (int i = 0; i < (int)equipo.size(); i++) {
        if (!equipo[i].estaMuerto()) return &equipo[i];
    }
    return nullptr; // equipo completamente debilitado
}

bool Jugador::tienePokemonVivo() const {
    for (int i = 0; i < (int)equipo.size(); i++) {
        if (!equipo[i].estaMuerto()) return true;
    }
    return false;
}

int Jugador::getTamano() { return (int)equipo.size(); }

void Jugador::agregarPlata(int cantidad) {
    plata += cantidad;
}

// Retorna false si no tiene suficiente plata
bool Jugador::gastarPlata(int cantidad) {
    if (plata < cantidad) return false;
    plata -= cantidad;
    return true;
}

void Jugador::agregarMedalla() {
    medallas++;
}

// Cura el equipo completo al maximo de HP
void Jugador::curarEquipo() {
    for (int i = 0; i < (int)equipo.size(); i++) {
        equipo[i].curarFull();
    }
}

bool Jugador::equipoLleno() const {
    return equipo.size() >= 6;
}
