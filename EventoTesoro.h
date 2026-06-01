//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOTESORO_H
#define PROYECTO_2_EVENTOTESORO_H
#include "Evento.h"
#include "Item.h"

class EventoTesoro:public Evento {
private:
    Item tesoro;
    public:
    EventoTesoro(const Item &tesoro);
    void ejecutar(Jugador& jug)override;
};


#endif //PROYECTO_2_EVENTOTESORO_H