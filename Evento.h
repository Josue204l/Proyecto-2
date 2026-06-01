//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTO_H
#define PROYECTO_2_EVENTO_H
#include "Jugador.h"

class Evento {
    public:
    virtual ~Evento() = default;
    virtual void ejecutar(Jugador& jugador) = 0;
};


#endif //PROYECTO_2_EVENTO_H