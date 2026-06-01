//
// Created by jr156 on 28/5/2026.
//

#include "Item.h"

Item::Item() : nombre("Desconocido"), tipo(TipoItem::POTION), valor(0) {}

Item::Item(std::string nombre, TipoItem tipo, int valor)
    : nombre(std::move(nombre)), tipo(tipo), valor(valor) {}

std::string Item::getNombre() { return nombre; }

TipoItem Item::getTipo() { return tipo; }

int Item::getValor() { return valor; }