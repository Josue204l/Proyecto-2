//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOGIMNASIO_H
#define PROYECTO_2_EVENTOGIMNASIO_H
#include "Entrenador.h"
#include "Evento.h"


class EventoGimnasio : public Evento {
    private:
    Entrenador liderGym;
    public:
    EventoGimnasio(const Entrenador & liderGym);
    void ejecutar(Jugador &juga)override ;
};


#endif //PROYECTO_2_EVENTOGIMNASIO_H