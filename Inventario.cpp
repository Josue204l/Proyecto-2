//
// Created by Gabri on 5/31/26.
//

#include "Inventario.h"

Inventario::Inventario() {}

void Inventario::agregarItem(Item& item) {
    items.push_back(item);
}

bool Inventario::removerItem(int index) {
    if (index < 0 || index >= (int)items.size()) return false;
    items.erase(items.begin() + index);
    return true;
}

Item Inventario::getItem(int index) {
    return items.at(index);
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

const std::vector<Item>& Inventario::getItems() const {
    return items;
}