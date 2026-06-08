//
// Created by jr156 on 28/5/2026.
//

#include "World.h"

World::World() {}

// Constructor de copia: hace copias profundas de cada Lugar
World::World(const World& otro) {
    for (Location* l : otro.lugares) {
        lugares.push_back(new Location(*l));
    }
}

// Operador de asignacion: libera los actuales y copia los nuevos
World& World::operator=(const World& otro) {
    if (this == &otro) return *this;
    for (Location* l : lugares) delete l;
    lugares.clear();
    for (Location* l : otro.lugares) {
        lugares.push_back(new Location(*l));
    }
    return *this;
}

void World::agregarLugar(Location* lugar) {
    lugares.push_back(lugar);
}

Location* World::getLugar(int id) {
    for (Location* l : lugares) {
        if (l->getId() == id) return l;
    }
    return nullptr;
}

const vector<Location*>& World::getLugares() const {
    return lugares;
}

World::~World() {
    for (Location* l : lugares) delete l;
    lugares.clear();
}
