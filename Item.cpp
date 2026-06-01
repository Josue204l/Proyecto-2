#include "Item.h"

Item::Item() : nombre("Desconocido"), tipo(TipoItem::POTION), valor(0) {
}

Item::Item(string nombre, TipoItem tipo, int valor) {
    this->nombre = nombre;
    this->tipo = tipo;
    this->valor = valor;
}

string Item::getNombre() { return nombre; }
TipoItem Item::getTipo() { return tipo; }
int Item::getValor() { return valor; }
