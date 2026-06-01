//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_OBJECTIVE_H
#define PROYECTO_2_OBJECTIVE_H
#include <string>

// Represents a win/progress condition for the simulation
class Objective {
private:
    std::string descripcion;
    bool completado;
public:
    Objective();
    explicit Objective(const std::string& desc);
    std::string getDescripcion() const;
    bool estaCompletado() const;
    void completar();
};

#endif //PROYECTO_2_OBJECTIVE_H