//
// Created by Gabri on 5/31/26.
//

#include "Jugador.h"

Jugador::Jugador() : nombre("Ash"), plata(500), medallas(0) {}

Jugador::Jugador(std::string nombre) : nombre(std::move(nombre)), plata(500), medallas(0) {}

std::string Jugador::getNombre() { return nombre; }

int Jugador::getPlata() { return plata; }

int Jugador::getMedallas() { return medallas; }

Inventario& Jugador::getInventario() { return inventario; }

const std::vector<Pokemon>& Jugador::getEquipo() const { return equipo; }

bool Jugador::agregarPokemon(Pokemon p) {
    if ((int)equipo.size() >= 6) return false;
    equipo.push_back(p);
    return true;
}

bool Jugador::quitarPokemon(int index) {
    if (index < 0 || index >= (int)equipo.size()) return false;
    equipo.erase(equipo.begin() + index);
    return true;
}

Pokemon* Jugador::getPokemon(int index) {
    if (index < 0 || index >= (int)equipo.size()) return nullptr;
    return &equipo[index];
}

Pokemon* Jugador::getPokemonActiv() {
    for (auto& p : equipo) {
        if (!p.estaMuerto()) return &p;
    }
    return nullptr;
}

bool Jugador::tienePokemonVivo() const {
    for (const auto& p : equipo) {
        if (!p.estaMuerto()) return true;
    }
    return false;
}

int Jugador::getTamano() { return (int)equipo.size(); }

void Jugador::agregarPlata(int cantidad) { plata += cantidad; }

bool Jugador::gastarPlata(int cantidad) {
    if (plata < cantidad) return false;
    plata -= cantidad;
    return true;
}

void Jugador::agregarMedalla() { medallas++; }

void Jugador::curarEquipo() {
    for (auto& p : equipo) p.curarFull();
}