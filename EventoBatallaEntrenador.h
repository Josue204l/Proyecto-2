//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOBATALLAENTRENADOR_H
#define PROYECTO_2_EVENTOBATALLAENTRENADOR_H
#include "Entrenador.h"
#include "Evento.h"

class EventoBatallaEntrenador : public Evento {
    private:
    Entrenador entrenador;
    public:
    EventoBatallaEntrenador(const Entrenador &ent);
    void ejecutar(Jugador &jug)override ;
};


#endif //PROYECTO_2_EVENTOBATALLAENTRENADOR_H