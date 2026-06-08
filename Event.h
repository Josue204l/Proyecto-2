//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTO_H
#define PROYECTO_2_EVENTO_H
#include "Player.h"

class Event {
    public:
    virtual ~Event() = default;
    virtual void ejecutar(Player& jugador) = 0;
};


#endif //PROYECTO_2_EVENTO_H