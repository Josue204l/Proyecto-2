#ifndef PROYECTO_2_INVENTARIO_H
#define PROYECTO_2_INVENTARIO_H
#include <vector>
#include "Item.h"

// Guarda los items que tiene el jugador
class Inventario {
private:
    vector<Item> items;

public:
    Inventario();

    void agregarItem(Item& item);
    bool removerItem(int indice);
    Item getItem(int indice);
    int getTamano() const;
    bool estaVacio() const;
    void limpiar();
    const vector<Item>& getItems() const;
};

#endif // PROYECTO_2_INVENTARIO_H
