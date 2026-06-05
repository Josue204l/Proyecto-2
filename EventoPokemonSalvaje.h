//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOPOKEMONSALVAJE_H
#define PROYECTO_2_EVENTOPOKEMONSALVAJE_H
#include "Evento.h"
#include "Pokemon.h"

class EventoPokemonSalvaje : public Evento{
    private:
    Pokemon p;
    bool intentarCaptura(Jugador& juga, Pokemon& salvaje);
    public:
    EventoPokemonSalvaje(const Pokemon &p);
    void ejecutar(Jugador& jugador)override;

};


#endif //PROYECTO_2_EVENTOPOKEMONSALVAJE_H