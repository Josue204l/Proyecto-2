#include "Player.h"

// Constructor por defecto, el jugador se llama Ash y empieza con 500 monedas
Player::Player() : nombre("Ash"), plata(500), medallas(0) {
}

Player::Player(string nombre) {
    this->nombre = nombre;
    this->plata = 500;
    this->medallas = 0;
}

string Player::getNombre() { return nombre; }
int Player::getPlata() { return plata; }
int Player::getMedallas() { return medallas; }
Inventory& Player::getInventario() { return inventario; }
const vector<Pokemon>& Player::getEquipo() const { return equipo; }

// Solo se pueden tener 6 pokemon en el equipo
bool Player::agregarPokemon(Pokemon p) {
    if ((int)equipo.size() >= 6) return false;
    equipo.push_back(p);
    return true;
}

bool Player::quitarPokemon(int indice) {
    if (indice < 0 || indice >= (int)equipo.size()) return false;
    equipo.erase(equipo.begin() + indice);
    return true;
}

Pokemon* Player::getPokemon(int indice) {
    if (indice < 0 || indice >= (int)equipo.size()) return nullptr;
    return &equipo[indice];
}

// Busca el primer pokemon que no este debilitado
Pokemon* Player::getPokemonActiv() {
    for (int i = 0; i < (int)equipo.size(); i++) {
        if (!equipo[i].estaMuerto()) return &equipo[i];
    }
    return nullptr; // equipo completamente debilitado
}

bool Player::tienePokemonVivo() const {
    for (int i = 0; i < (int)equipo.size(); i++) {
        if (!equipo[i].estaMuerto()) return true;
    }
    return false;
}

int Player::getTamano() { return (int)equipo.size(); }

void Player::agregarPlata(int cantidad) {
    plata += cantidad;
}

// Retorna false si no tiene suficiente plata
bool Player::gastarPlata(int cantidad) {
    if (plata < cantidad) return false;
    plata -= cantidad;
    return true;
}

void Player::agregarMedalla() {
    medallas++;
}

// Cura el equipo completo al maximo de HP
void Player::curarEquipo() {
    for (int i = 0; i < (int)equipo.size(); i++) {
        equipo[i].curarFull();
    }
}

bool Player::equipoLleno() const {
    return equipo.size() >= 6;
}
