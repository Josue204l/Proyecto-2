//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_OBSTACLE_H
#define PROYECTO_2_OBSTACLE_H
#include <string>

// Represents a condition that blocks or penalizes the player
class Obstacle {
private:
    std::string descripcion;
    int dannio;
public:
    Obstacle();
    Obstacle(const std::string& desc, int dannio);
    std::string getDescripcion() const;
    int getDannio() const;
};

#endif //PROYECTO_2_OBSTACLE_H