//
// Created by jr156 on 28/5/2026.
//

#include "Objective.h"

Objective::Objective() : descripcion("Sin descripcion"), completado(false) {}

Objective::Objective(const std::string& desc) : descripcion(desc), completado(false) {}

std::string Objective::getDescripcion() const { return descripcion; }

bool Objective::estaCompletado() const { return completado; }

void Objective::completar() { completado = true; }