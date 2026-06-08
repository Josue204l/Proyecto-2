//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_PELEASTRATEGY_H
#define PROYECTO_2_PELEASTRATEGY_H


class BattleStrategy {
    public:
    virtual ~BattleStrategy() = default;
    virtual int elegirAccion() = 0;
};


#endif //PROYECTO_2_PELEASTRATEGY_H