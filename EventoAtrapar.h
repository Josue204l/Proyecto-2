//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOATRAPAR_H
#define PROYECTO_2_EVENTOATRAPAR_H

#include "Evento.h"
class EventoAtrapar : public Evento {
    private:
    int dannio;
    public:
    EventoAtrapar(int dannio);
    void ejecutar(Jugador &juga)override;
};


#endif //PROYECTO_2_EVENTOATRAPAR_H