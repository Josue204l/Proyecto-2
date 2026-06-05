//
// Created by Gabri on 5/31/26.
//

#include "EventoAtrapar.h"
#include "Logger.h"

EventoAtrapar::EventoAtrapar(int dannio) {
    this->dannio = dannio;
}

void EventoAtrapar::ejecutar(Jugador& juga) {
    Logger::getInstancia().log("[Trampa] ¡Caíste en una trampa! Recibes " + std::to_string(dannio) + " de daño.");
    Pokemon* activo = juga.getPokemonActiv();
    if (activo) {
        activo->dannioRecibido(dannio);
        Logger::getInstancia().log("[Trampa] " + activo->getNombre() + " queda con " + std::to_string(activo->getHpActual()) + " HP.");
    }
}
