//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOATRAPAR_H
#define PROYECTO_2_EVENTOATRAPAR_H

#include "Event.h"
class TrapEvent : public Event {
    private:
    int dannio;
    public:
    TrapEvent(int dannio);
    void ejecutar(Player &juga)override;
};


#endif //PROYECTO_2_EVENTOATRAPAR_H