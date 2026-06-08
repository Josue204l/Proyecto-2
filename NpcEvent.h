//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTONPC_H
#define PROYECTO_2_EVENTONPC_H
#include <string>

#include "Event.h"


class NpcEvent : public Event {
    private:
    std::string mensaje;
    public:
    NpcEvent(const std::string & mensaje);
    void ejecutar(Player &jugador) override;
};


#endif //PROYECTO_2_EVENTONPC_H