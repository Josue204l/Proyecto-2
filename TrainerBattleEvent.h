//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOBATALLAENTRENADOR_H
#define PROYECTO_2_EVENTOBATALLAENTRENADOR_H
#include "Trainer.h"
#include "Event.h"

class TrainerBattleEvent : public Event {
    private:
    Trainer entrenador;
    public:
    TrainerBattleEvent(const Trainer &ent);
    void ejecutar(Player &jug)override ;
};


#endif //PROYECTO_2_EVENTOBATALLAENTRENADOR_H