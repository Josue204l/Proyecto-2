//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_ITEM_H
#define PROYECTO_2_ITEM_H
#include <string>

#include "Enums.h"


class Item {
    private:
    std::string nombre;
    TipoItem tipo;
    int valor;
    public:
    Item();
    Item(std::string nombre, TipoItem tipo, int valor);
    std::string getNombre();
    TipoItem getTipo();
    int getValor();

};


#endif //PROYECTO_2_ITEM_H