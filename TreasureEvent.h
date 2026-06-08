//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOTESORO_H
#define PROYECTO_2_EVENTOTESORO_H
#include "Event.h"
#include "Item.h"

class TreasureEvent:public Event {
private:
    Item tesoro;
    public:
    TreasureEvent(const Item &tesoro);
    void ejecutar(Player& jug)override;
};


#endif //PROYECTO_2_EVENTOTESORO_H