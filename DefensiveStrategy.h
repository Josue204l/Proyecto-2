//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_DEFENSASTRATEGY_H
#define PROYECTO_2_DEFENSASTRATEGY_H
#include "BattleStrategy.h"


class DefensiveStrategy : public BattleStrategy {
    public:
    int elegirAccion() override;
};


#endif //PROYECTO_2_DEFENSASTRATEGY_H