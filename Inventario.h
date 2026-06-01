//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_INVENTARIO_H
#define PROYECTO_2_INVENTARIO_H
#include <vector>
#include "Item.h"


class Inventario {
    private:
    std::vector<Item> items;
    public:
    Inventario();
    void agregarItem(Item &item);
    bool removerItem(int index);
    Item getItem(int index);
    int getTamano() const;
    bool estaVacio()const;
    void limpiar();
    const std::vector<Item>& getItems() const;

};


#endif //PROYECTO_2_INVENTARIO_H