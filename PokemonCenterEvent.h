//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOCENTROPOKEMON_H
#define PROYECTO_2_EVENTOCENTROPOKEMON_H
#include "Event.h"

class PokemonCenterEvent:public Event {
    public:
    PokemonCenterEvent();
    void ejecutar(Player &jug)override ;
};


#endif //PROYECTO_2_EVENTOCENTROPOKEMON_H