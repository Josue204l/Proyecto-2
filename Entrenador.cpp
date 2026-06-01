//
// Created by Gabri on 5/31/26.
//

#include "Entrenador.h"

Entrenador::Entrenador() : nombre("Entrenador"), plataGanada(100) {}

Entrenador::Entrenador(std::string nombre, int plataGanada)
    : nombre(std::move(nombre)), plataGanada(plataGanada) {}

std::string Entrenador::getNombre() { return nombre; }

int Entrenador::getPlataGanada() { return plataGanada; }

const std::vector<Pokemon>& Entrenador::getEquipo() { return equipo; }

void Entrenador::addPokemon(Pokemon& p) { equipo.push_back(p); }

Pokemon& Entrenador::getPokemon(int index) { return equipo.at(index); }

bool Entrenador::tienePokemonVivos() const {
    for (const auto& p : equipo) {
        if (!p.estaMuerto()) return true;
    }
    return false;
}