//
// Created by Gabri on 5/31/26.
//

#include "GeneradorMapa.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Helper: parse TipoEvento from string
static TipoEvento parseTipoEvento(const std::string& s) {
    if (s == "WILD_POKEMON")   return TipoEvento::WILD_POKEMON;
    if (s == "TRAINER")        return TipoEvento::TRAINER;
    if (s == "SHOP")           return TipoEvento::SHOP;
    if (s == "POKEMON_CENTER") return TipoEvento::POKEMON_CENTER;
    if (s == "TREASURE")       return TipoEvento::TREASURE;
    if (s == "TRAP")           return TipoEvento::TRAP;
    if (s == "NPC")            return TipoEvento::NPC;
    if (s == "GYM")            return TipoEvento::GYM;
    if (s == "LEAGUE")         return TipoEvento::LEAGUE;
    return TipoEvento::NPC;
}

// Map file format per line: id,nombre,tipoEvento,conexion1,conexion2,...
World GeneradorMapa::generarMundo(const std::string& archivoMapa) {
    World world;
    std::ifstream file(archivoMapa);
    if (!file.is_open()) {
        std::cerr << "[GeneradorMapa] No se pudo abrir: " << archivoMapa << std::endl;
        return world;
    }
    std::string linea;
    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        std::stringstream ss(linea);
        std::string token, nombre, tipoStr;
        int id;
        try {
            std::getline(ss, token, ','); id = std::stoi(token);
            std::getline(ss, nombre, ',');
            std::getline(ss, tipoStr, ',');
            Lugar* lugar = new Lugar(id, nombre, parseTipoEvento(tipoStr));
            while (std::getline(ss, token, ',')) {
                if (!token.empty()) lugar->agregarConexion(std::stoi(token));
            }
            world.agregarLugar(lugar);
        } catch (...) {
            std::cerr << "[GeneradorMapa] Error al parsear linea: " << linea << std::endl;
        }
    }
    file.close();
    return world;
}