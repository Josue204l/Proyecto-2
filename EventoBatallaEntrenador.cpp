//
// Created by Gabri on 5/31/26.
//

#include "EventoBatallaEntrenador.h"
#include "Pelea.h"
#include "Logger.h"

EventoBatallaEntrenador::EventoBatallaEntrenador(const Entrenador& ent) : entrenador(ent) {}

void EventoBatallaEntrenador::ejecutar(Jugador& jug) {
    Logger::getInstancia().log("[Evento] Entrenador " + entrenador.getNombre() + " ¡quiere batallar!");
    Pelea::empezarBatallaEntrenador(jug, entrenador);
}
