//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_CHARACTER_H
#define PROYECTO_2_CHARACTER_H
#include <string>

class Character {
public:
    virtual ~Character() = default;
    virtual std::string getNombre() = 0;
};


#endif //PROYECTO_2_CHARACTER_H