//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_RANDOMSTRATEGY_H
#define PROYECTO_2_RANDOMSTRATEGY_H
#include "PeleaStrategy.h"


class RandomStrategy : public PeleaStrategy {
public :
    int elegirAccion() override;
};


#endif //PROYECTO_2_RANDOMSTRATEGY_H