//
// Created by jr156 on 28/5/2026.
//

#include "World.h"

World::World() {}

void World::agregarLugar(Lugar* lugar) {
    lugares.push_back(lugar);
}

Lugar* World::getLugar(int id) {
    for (auto* l : lugares) {
        if (l->getId() == id) return l;
    }
    return nullptr;
}

const std::vector<Lugar*>& World::getLugares() const {
    return lugares;
}

World::~World() {
    for (auto* l : lugares) delete l;
    lugares.clear();
}