//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_AGRESIVASTRATEGY_H
#define PROYECTO_2_AGRESIVASTRATEGY_H
#include "PeleaStrategy.h"


class AgresivaStrategy : public PeleaStrategy{
    public:
    int elegirAccion() override;
};


#endif //PROYECTO_2_AGRESIVASTRATEGY_H