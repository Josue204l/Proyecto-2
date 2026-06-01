//
// Created by Gabri on 5/31/26.
//

#include "Lugar.h"

Lugar::Lugar() : id(0), nombre("Desconocido"), tipo(TipoEvento::NPC) {}

Lugar::Lugar(int id, const std::string& nombre, TipoEvento tipo)
    : id(id), nombre(nombre), tipo(tipo) {}

int Lugar::getId() const { return id; }

std::string Lugar::getNombre() const { return nombre; }

TipoEvento Lugar::getTipoEvento() const { return tipo; }

void Lugar::agregarConexion(int idLugar) {
    siguienteLugar.push_back(idLugar);
}

const std::vector<int>& Lugar::getSiguienteLugar() const {
    return siguienteLugar;
}

const std::vector<int>& Lugar::getConexiones() const {
    return siguienteLugar;
}