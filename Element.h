#ifndef PROYECTO_2_ELEMENT_H
#define PROYECTO_2_ELEMENT_H
#include <string>

// Clase base abstracta para elementos interactivos del mundo (items, obstaculos, etc.)
class Element {
public:
    virtual ~Element() = default;
    virtual string getNombre() const = 0;
    virtual string getDescripcion() const = 0;
};

#endif // PROYECTO_2_ELEMENT_H
