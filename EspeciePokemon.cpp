#include "EspeciePokemon.h"

// Constructor por defecto, crea una especie vacia
EspeciePokemon::EspeciePokemon()
    : nombre(""), tipo(TipoPokemon::NORMAL),
      hpBase(0), ataqueBase(0), defensaBase(0),
      nivelEvolucion(0), nombreEvolucion(""), rareza(Rareza::COMMON), starter(false) {}

// Constructor con los datos completos de la especie
EspeciePokemon::EspeciePokemon(const string& nombre, TipoPokemon tipo, int hpBase,
                               int ataqueBase, int defensaBase, int nivelEvolucion,
                               const string& nombreEvolucion, Rareza rareza) {
    this->nombre = nombre;
    this->tipo = tipo;
    this->hpBase = hpBase;
    this->ataqueBase = ataqueBase;
    this->defensaBase = defensaBase;
    this->nivelEvolucion = nivelEvolucion;
    this->nombreEvolucion = nombreEvolucion;
    this->rareza = rareza;
}

string EspeciePokemon::getNombre() const { return nombre; }
TipoPokemon EspeciePokemon::getTipo() const { return tipo; }
int EspeciePokemon::getHpBase() const { return hpBase; }
int EspeciePokemon::getAtaqueBase() const { return ataqueBase; }
int EspeciePokemon::getDefenseBase() const { return defensaBase; }
int EspeciePokemon::getNivelEvolucion() const { return nivelEvolucion; }
string EspeciePokemon::getNombreEvolucion() const { return nombreEvolucion; }
Rareza EspeciePokemon::getRareza() const { return rareza; }
bool EspeciePokemon::esStarter() const {return starter;}
void EspeciePokemon::setStarter(bool valor) {
    starter = valor;
}
