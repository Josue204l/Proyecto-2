//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_DEFENSASTRATEGY_H
#define PROYECTO_2_DEFENSASTRATEGY_H
#include "PeleaStrategy.h"


class DefensaStrategy : public PeleaStrategy {
    public:
    int elegirAccion() override;
};


#endif //PROYECTO_2_DEFENSASTRATEGY_H