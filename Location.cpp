//
// Created by Gabri on 5/31/26.
//

#include "Location.h"

Location::Location() : id(0), nombre("Desconocido"), tipo(TipoEvento::NPC) {}

Location::Location(int id, const std::string& nombre, TipoEvento tipo)
    : id(id), nombre(nombre), tipo(tipo) {}

int Location::getId() const { return id; }

std::string Location::getNombre() const { return nombre; }

TipoEvento Location::getTipoEvento() const { return tipo; }

void Location::agregarConexion(int idLugar) {
    siguienteLugar.push_back(idLugar);
}

const std::vector<int>& Location::getSiguienteLugar() const {
    return siguienteLugar;
}

const std::vector<int>& Location::getConexiones() const {
    return siguienteLugar;
}