//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_RANDOMSTRATEGY_H
#define PROYECTO_2_RANDOMSTRATEGY_H
#include "BattleStrategy.h"


class RandomStrategy : public BattleStrategy {
public :
    int elegirAccion() override;
};


#endif //PROYECTO_2_RANDOMSTRATEGY_H