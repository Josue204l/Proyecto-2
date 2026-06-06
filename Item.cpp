#include "Item.h"

Item::Item() : nombre("Desconocido"), tipo(TipoItem::POTION), valor(0) {
    // Le pongo defaults decentes para que un item incompleto no truene media partida por datos basura.
}

Item::Item(string nombre, TipoItem tipo, int valor) {
    // Acá no valido demasiado porque asumo que el loader ya hizo el trabajo sucio antes.
    this->nombre = nombre;
    this->tipo = tipo;
    this->valor = valor;
}

string Item::getNombre() { return nombre; }
TipoItem Item::getTipo() { return tipo; }
int Item::getValor() { return valor; }
