#include "Inventario.h"

Inventario::Inventario() {
    // empieza vacio
}

void Inventario::agregarItem(Item& item) {
    items.push_back(item);
}

// Elimina el item en la posicion indicada, retorna false si el indice no existe
bool Inventario::removerItem(int indice) {
    if (indice < 0 || indice >= (int)items.size()) return false;
    items.erase(items.begin() + indice);
    return true;
}

Item Inventario::getItem(int indice) {
    return items.at(indice);
}

int Inventario::getTamano() const {
    return (int)items.size();
}

bool Inventario::estaVacio() const {
    return items.empty();
}

void Inventario::limpiar() {
    items.clear();
}

const vector<Item>& Inventario::getItems() const {
    return items;
}
