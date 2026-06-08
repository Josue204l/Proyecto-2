//
// Created by jr156 on 28/5/2026.
//

#include "FileLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

FileLoader::FileLoader() {}

// Mini helper: convierto el texto a enum acá para no repetir el mismo if raro por todo el loader.
static TipoPokemon parseTipo(const std::string& s) {
    if (s == "FIRE")     return TipoPokemon::FIRE;
    if (s == "WATER")    return TipoPokemon::WATER;
    if (s == "GRASS")    return TipoPokemon::GRASS;
    if (s == "ELECTRIC") return TipoPokemon::ELECTRIC;
    if (s == "ROCK")     return TipoPokemon::ROCK;
    if (s == "FLYING")   return TipoPokemon::FLYING;
    return TipoPokemon::NORMAL;
}

// Lo dejo separado porque la rareza cambia bastante el balance y prefiero esa lógica bien localizada.
static Rareza parseRareza(const std::string& s) {
    if (s == "RARE")      return Rareza::RARE;
    if (s == "EPIC")      return Rareza::EPIC;
    if (s == "LEGENDARY") return Rareza::LEGENDARY;
    return Rareza::COMMON;
}

// Mismo truco con items; asi el archivo de carga principal no se ensucia tanto.
static TipoItem parseTipoItem(const std::string& s) {
    if (s == "POKEBALL")     return TipoItem::POKEBALL;
    if (s == "GREATBALL")    return TipoItem::GREATBALL;
    if (s == "SUPER_POTION") return TipoItem::SUPER_POTION;
    if (s == "RARE_CANDY")   return TipoItem::RARE_CANDY;
    return TipoItem::POTION;
}

// Este formato queda fijo porque simplifica el parseo y evita andar adivinando columnas.
bool FileLoader::cargarEspecies(const std::string& nombreArchivo) {
    std::ifstream file(nombreArchivo);
    if (!file.is_open()) {
        std::cerr << "[FileLoader] No se pudo abrir: " << nombreArchivo << std::endl;
        return false;
    }
    std::string linea;
    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        std::stringstream ss(linea);
        std::string nombre, tipo, evo, rareza;
        int hp, atk, def, nivEvo;
        try {
            std::getline(ss, nombre, ',');
            std::getline(ss, tipo, ',');
            ss >> hp; ss.ignore();
            ss >> atk; ss.ignore();
            ss >> def; ss.ignore();
            ss >> nivEvo; ss.ignore();
            std::getline(ss, evo, ',');
            std::getline(ss, rareza, ',');
            std::string starterStr;
            std::getline(ss, starterStr, ',');
            EspeciePokemon esp(nombre, parseTipo(tipo), hp, atk, def, nivEvo, evo, parseRareza(rareza));
            esp.setStarter(starterStr == "true");
            especie.push_back(esp);
        } catch (...) {
            std::cerr << "[FileLoader] Error al parsear especie: " << linea << std::endl;
        }
    }
    file.close();
    return !especie.empty();
}

// Cortito a propósito: mientras menos columnas tenga el item, menos chance de meter datos mal escritos.
bool FileLoader::cargarItem(const std::string& nombreArchivo) {
    std::ifstream file(nombreArchivo);
    if (!file.is_open()) {
        std::cerr << "[FileLoader] No se pudo abrir: " << nombreArchivo << std::endl;
        return false;
    }
    std::string linea;
    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        std::stringstream ss(linea);
        std::string nombre, tipo;
        int valor;
        try {
            std::getline(ss, nombre, ',');
            std::getline(ss, tipo, ',');
            ss >> valor;
            items.emplace_back(nombre, parseTipoItem(tipo), valor);
        } catch (...) {
            std::cerr << "[FileLoader] Error al parsear item: " << linea << std::endl;
        }
    }
    file.close();
    return !items.empty();
}

// Acá se usa este formato porque deja definir equipos rápido sin hacer otra estructura más pesada.
bool FileLoader::cargarLiderGym(const std::string& nombreArchivo) {
    std::ifstream file(nombreArchivo);
    if (!file.is_open()) {
        std::cerr << "[FileLoader] No se pudo abrir: " << nombreArchivo << std::endl;
        return false;
    }
    std::string linea;
    while (std::getline(file, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        std::stringstream ss(linea);
        std::string nombre, token;
        int plata;
        try {
            std::getline(ss, nombre, ',');
            ss >> plata; ss.ignore();
            Entrenador ent(nombre, plata);
            while (std::getline(ss, token, ',')) {
                std::stringstream ps(token);
                std::string nomPoke;
                int nivel;
                std::getline(ps, nomPoke, ':');
                ps >> nivel;
                // Find species by name
                for (const auto& esp : especie) {
                    if (esp.getNombre() == nomPoke) {
                        Pokemon p(esp, nivel);
                        ent.addPokemon(p);
                        break;
                    }
                }
            }
            liderGym.push_back(ent);
        } catch (...) {
            std::cerr << "[FileLoader] Error al parsear lider: " << linea << std::endl;
        }
    }
    file.close();
    return !liderGym.empty();
}

const std::vector<EspeciePokemon>& FileLoader::getEspecie() const { return especie; }
const std::vector<Item>& FileLoader::getItems() const { return items; }
const std::vector<Entrenador>& FileLoader::getLiderGym() const { return liderGym; }
