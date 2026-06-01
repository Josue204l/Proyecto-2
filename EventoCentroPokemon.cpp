//
// Created by Gabri on 5/31/26.
//

#include "EventoCentroPokemon.h"
#include "Logger.h"

EventoCentroPokemon::EventoCentroPokemon() {}

void EventoCentroPokemon::ejecutar(Jugador& jug) {
    Logger::getInstancia().log("[Centro Pokemon] Bienvenido! Curamos a todos tus Pokemon.");
    jug.curarEquipo();
    Logger::getInstancia().log("[Centro Pokemon] Todos tus Pokemon estan completamente curados.");
}