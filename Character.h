#ifndef PROYECTO_2_CHARACTER_H
#define PROYECTO_2_CHARACTER_H
#include <string>
using namespace std;

// Clase base abstracta para cualquier personaje del juego
// Jugador y Entrenador heredan de esta clase
class Character {
public:
    virtual ~Character() = default;
    virtual string getNombre() = 0; // cada personaje debe tener nombre
};

#endif // PROYECTO_2_CHARACTER_H
