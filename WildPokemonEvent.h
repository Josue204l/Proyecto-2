//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOPOKEMONSALVAJE_H
#define PROYECTO_2_EVENTOPOKEMONSALVAJE_H
#include "Event.h"
#include "Pokemon.h"

class WildPokemonEvent : public Event{
    private:
    Pokemon p;
    bool intentarCaptura(Player& juga, Pokemon& salvaje);
    public:
    WildPokemonEvent(const Pokemon &p);
    void ejecutar(Player& jugador)override;

};


#endif //PROYECTO_2_EVENTOPOKEMONSALVAJE_H