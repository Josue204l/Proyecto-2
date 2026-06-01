//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_SPACE_H
#define PROYECTO_2_SPACE_H
#include <string>
#include <vector>

// Abstract base for any navigable location in the world
class Space {
public:
    virtual ~Space() = default;
    virtual int getId() const = 0;
    virtual std::string getNombre() const = 0;
    virtual const std::vector<int>& getConexiones() const = 0;
};

#endif //PROYECTO_2_SPACE_H