//
// Created by Gabri on 5/31/26.
//

#include "EventoNPC.h"
#include "Logger.h"

EventoNPC::EventoNPC(const std::string& mensaje) : mensaje(mensaje) {}

void EventoNPC::ejecutar(Jugador& jugador) {
    Logger::getInstancia().log("[NPC] " + mensaje);
}