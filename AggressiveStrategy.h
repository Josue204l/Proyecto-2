//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_AGRESIVASTRATEGY_H
#define PROYECTO_2_AGRESIVASTRATEGY_H
#include "BattleStrategy.h"


class AggressiveStrategy : public BattleStrategy{
    public:
    int elegirAccion() override;
};


#endif //PROYECTO_2_AGRESIVASTRATEGY_H