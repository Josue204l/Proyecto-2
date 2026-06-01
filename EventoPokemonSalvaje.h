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
    public:
    EventoPokemonSalvaje(const Pokemon &p);
    void ejecutar(Jugador& jugador)override;
};


#endif //PROYECTO_2_EVENTOPOKEMONSALVAJE_H