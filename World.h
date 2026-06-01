//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_WORLD_H
#define PROYECTO_2_WORLD_H

#include "Lugar.h"
class World {
    private :
    std::vector<Lugar*> lugares;
    public :
    World();
    void agregarLugar(const Lugar* lugar);
    Lugar* getLugar(int id);
    const std::vector<Lugar*>& getLugares() const;
};


#endif //PROYECTO_2_WORLD_H