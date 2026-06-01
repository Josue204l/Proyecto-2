//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOCENTROPOKEMON_H
#define PROYECTO_2_EVENTOCENTROPOKEMON_H
#include "Evento.h"

class EventoCentroPokemon:public Evento {
    public:
    EventoCentroPokemon();
    void ejecutar(Jugador &jug)override ;
};


#endif //PROYECTO_2_EVENTOCENTROPOKEMON_H