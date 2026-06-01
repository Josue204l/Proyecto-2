//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_EVENTOTIENDAOBJETOS_H
#define PROYECTO_2_EVENTOTIENDAOBJETOS_H
#include <vector>
#include "Evento.h"
#include "Item.h"


class EventoTiendaObjetos : public Evento {
    private:
    std::vector<Item> tiendaItems;
    public:
    EventoTiendaObjetos(const std::vector<Item> &tienda);
    void ejecutar(Jugador& jugador)override;
};


#endif //PROYECTO_2_EVENTOTIENDAOBJETOS_H