#ifndef PROYECTO_2_ITEM_H
#define PROYECTO_2_ITEM_H
#include <string>
#include "Enums.h"
using namespace std;

// Representa un objeto que el jugador puede tener en su inventario
class Item {
private:
    string nombre;
    TipoItem tipo;
    int valor; // precio en monedas

public:
    Item();
    Item(string nombre, TipoItem tipo, int valor);

    string getNombre();
    TipoItem getTipo();
    int getValor();
};

#endif // PROYECTO_2_ITEM_H
