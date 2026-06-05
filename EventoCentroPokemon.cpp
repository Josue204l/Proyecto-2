//
// Created by Gabri on 5/31/26.
//

#include "EventoCentroPokemon.h"
#include "Logger.h"

EventoCentroPokemon::EventoCentroPokemon() {}

void EventoCentroPokemon::ejecutar(Jugador& jug) {
    Logger::getInstancia().log("[Centro Pokémon] ¡Bienvenido! Curamos a tu equipo.");
    jug.curarEquipo();
    Logger::getInstancia().log("[Centro Pokémon] Tu equipo está completamente curado.");
}
