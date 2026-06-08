#include "PokemonSpecies.h"

// Constructor por defecto, crea una especie vacia
PokemonSpecies::PokemonSpecies()
    : nombre(""), tipo(TipoPokemon::NORMAL),
      hpBase(0), ataqueBase(0), defensaBase(0),
      nivelEvolucion(0), nombreEvolucion(""), rareza(Rareza::COMMON), starter(false) {}

// Constructor con los datos completos de la especie
PokemonSpecies::PokemonSpecies(const string& nombre, TipoPokemon tipo, int hpBase,
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

string PokemonSpecies::getNombre() const { return nombre; }
TipoPokemon PokemonSpecies::getTipo() const { return tipo; }
int PokemonSpecies::getHpBase() const { return hpBase; }
int PokemonSpecies::getAtaqueBase() const { return ataqueBase; }
int PokemonSpecies::getDefenseBase() const { return defensaBase; }
int PokemonSpecies::getNivelEvolucion() const { return nivelEvolucion; }
string PokemonSpecies::getNombreEvolucion() const { return nombreEvolucion; }
Rareza PokemonSpecies::getRareza() const { return rareza; }
bool PokemonSpecies::esStarter() const {return starter;}
void PokemonSpecies::setStarter(bool valor) {
    starter = valor;
}
