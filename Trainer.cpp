//
// Created by Gabri on 5/31/26.
//

#include "Trainer.h"

Trainer::Trainer() : nombre("Entrenador"), plataGanada(100) {}

Trainer::Trainer(std::string nombre, int plataGanada)
    : nombre(std::move(nombre)), plataGanada(plataGanada) {}

std::string Trainer::getNombre() { return nombre; }

int Trainer::getPlataGanada() { return plataGanada; }

const std::vector<Pokemon>& Trainer::getEquipo() { return equipo; }

void Trainer::addPokemon(const Pokemon& p) { equipo.push_back(p); }

Pokemon& Trainer::getPokemon(int index) { return equipo.at(index); }

bool Trainer::tienePokemonVivos() const {
    for (const auto& p : equipo) {
        if (!p.estaMuerto()) return true;
    }
    return false;
}
