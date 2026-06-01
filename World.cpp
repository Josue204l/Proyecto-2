//
// Created by jr156 on 28/5/2026.
//

#include "World.h"

World::World() {}

// Constructor de copia: hace copias profundas de cada Lugar
World::World(const World& otro) {
    for (Lugar* l : otro.lugares) {
        lugares.push_back(new Lugar(*l));
    }
}

// Operador de asignacion: libera los actuales y copia los nuevos
World& World::operator=(const World& otro) {
    if (this == &otro) return *this;
    for (Lugar* l : lugares) delete l;
    lugares.clear();
    for (Lugar* l : otro.lugares) {
        lugares.push_back(new Lugar(*l));
    }
    return *this;
}

void World::agregarLugar(Lugar* lugar) {
    lugares.push_back(lugar);
}

Lugar* World::getLugar(int id) {
    for (Lugar* l : lugares) {
        if (l->getId() == id) return l;
    }
    return nullptr;
}

const vector<Lugar*>& World::getLugares() const {
    return lugares;
}

World::~World() {
    for (Lugar* l : lugares) delete l;
    lugares.clear();
}