//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_WORLD_H
#define PROYECTO_2_WORLD_H

#include "Location.h"
using namespace std;

class World {
private:
    vector<Location*> lugares;
public:
    World();
    ~World();

    // Constructor de copia y operador = para evitar doble delete de punteros
    World(const World& otro);
    World& operator=(const World& otro);

    void agregarLugar(Location* lugar);
    Location* getLugar(int id);
    const vector<Location*>& getLugares() const;
};

#endif // PROYECTO_2_WORLD_H