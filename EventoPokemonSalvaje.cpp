//
// Created by Gabri on 5/31/26.
//

#include "EventoPokemonSalvaje.h"
#include "Pelea.h"
#include "Logger.h"

EventoPokemonSalvaje::EventoPokemonSalvaje(const Pokemon& p) : p(p) {}

void EventoPokemonSalvaje::ejecutar(Jugador& jugador) {
    Logger::getInstancia().log("[Evento] Aparece un Pokemon salvaje: " + p.getNombre() + " (Nivel " + std::to_string(p.getNivel()) + ")");
    Pelea::empezarBatallaSalvaje(jugador, p);
}