//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_WORLD_H
#define PROYECTO_2_WORLD_H

#include "Lugar.h"
using namespace std;

class World {
private:
    vector<Lugar*> lugares;
public:
    World();
    ~World();

    // Constructor de copia y operador = para evitar doble delete de punteros
    World(const World& otro);
    World& operator=(const World& otro);

    void agregarLugar(Lugar* lugar);
    Lugar* getLugar(int id);
    const vector<Lugar*>& getLugares() const;
};

#endif // PROYECTO_2_WORLD_H