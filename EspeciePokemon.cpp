//
// Created by Gabri on 5/31/26.
//

#include "EspeciePokemon.h"

EspeciePokemon::EspeciePokemon() : nombre ("Desconocido"),tipo(TipoPokemon::NORMAL),hpBase(0),ataqueBase(0),defensaBase(0),nivelEvolucion(0),nombreEvolucion("Nada"),rarity(Rareza::COMMON){}

EspeciePokemon::EspeciePokemon(const string &nom, TipoPokemon tip, int hpb, int ataBase, int defeBase, int nivEvo, const string &nombEvo, Rareza rar) {
    this->nombre = nom;
    this->tipo = tip;
    this->hpBase = hpb;
    this->ataqueBase = ataBase;
    this->defensaBase = defeBase;
    this->nivelEvolucion = nivEvo;
    this->nombreEvolucion = nombEvo;
    this->rarity = rar;
}
std::string EspeciePokemon::getNombre()const {
    return this->nombre;
}
TipoPokemon EspeciePokemon::getTipo()const {
    return this->tipo;
}
int EspeciePokemon::getHpBase()const {
    return this->hpBase;
}
int EspeciePokemon::getAtaqueBase()const {
    return this->ataqueBase;
}

int EspeciePokemon::getDefenseBase() const {
     return this->defensaBase;
}
int EspeciePokemon::getNivelEvolucion()const {
    return this->nivelEvolucion;
}
std::string EspeciePokemon::getNombreEvolucion()const {
    return this->nombreEvolucion;
}
Rareza EspeciePokemon::getRareza()const {
    return this->rarity;
}