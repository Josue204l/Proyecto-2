//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOGIMNASIO_H
#define PROYECTO_2_EVENTOGIMNASIO_H
#include "Trainer.h"
#include "Event.h"


class GymEvent : public Event {
    private:
    Trainer liderGym;
    public:
    GymEvent(const Trainer & liderGym);
    void ejecutar(Player &juga)override ;
};


#endif //PROYECTO_2_EVENTOGIMNASIO_H