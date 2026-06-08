#include "Inventory.h"

Inventory::Inventory() {
    // empieza vacio
}

void Inventory::agregarItem(const Item& item) {
    items.push_back(item);
}

// Elimina el item en la posicion indicada, retorna false si el indice no existe
bool Inventory::removerItem(int indice) {
    if (indice < 0 || indice >= (int)items.size()) return false;
    items.erase(items.begin() + indice);
    return true;
}

Item Inventory::getItem(int indice) {
    return items.at(indice);
}

int Inventory::getTamano() const {
    return (int)items.size();
}

bool Inventory::estaVacio() const {
    return items.empty();
}

void Inventory::limpiar() {
    items.clear();
}

const vector<Item>& Inventory::getItems() const {
    return items;
}
