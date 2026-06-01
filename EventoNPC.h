//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTONPC_H
#define PROYECTO_2_EVENTONPC_H
#include <string>

#include "Evento.h"


class EventoNPC : public Evento {
    private:
    std::string mensaje;
    public:
    EventoNPC(const std::string & mensaje);
    void ejecutar(Jugador &jugador) override;
};


#endif //PROYECTO_2_EVENTONPC_H