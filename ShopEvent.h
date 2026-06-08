//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOTIENDAOBJETOS_H
#define PROYECTO_2_EVENTOTIENDAOBJETOS_H
#include <vector>
#include "Event.h"
#include "Item.h"


class ShopEvent : public Event {
    private:
    std::vector<Item> tiendaItems;
    public:
    ShopEvent(const std::vector<Item> &tienda);
    void ejecutar(Player& jugador)override;
};


#endif //PROYECTO_2_EVENTOTIENDAOBJETOS_H