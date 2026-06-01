//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_ELEMENT_H
#define PROYECTO_2_ELEMENT_H
#include <string>

// Abstract base for any interactive element in the world (items, obstacles, etc.)
class Element {
public:
    virtual ~Element() = default;
    virtual std::string getNombre() const = 0;
    virtual std::string getDescripcion() const = 0;
};

#endif //PROYECTO_2_ELEMENT_H