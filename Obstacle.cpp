//
// Created by jr156 on 28/5/2026.
//

#include "Obstacle.h"

Obstacle::Obstacle() : descripcion("Obstaculo"), dannio(0) {}

Obstacle::Obstacle(const std::string& desc, int d) : descripcion(desc), dannio(d) {}

std::string Obstacle::getDescripcion() const { return descripcion; }

int Obstacle::getDannio() const { return dannio; }