//
// Created by Gabri on 5/31/26.
//

#include "EventoAtrapar.h"
#include "Logger.h"

EventoAtrapar::EventoAtrapar(int dannio) {
    this->dannio = dannio;
}

// Trampa: hace danno directo al pokemon activo del jugador
void EventoAtrapar::ejecutar(Jugador& juga) {
    Logger::getInstancia().log("[Trampa] Caiste en una trampa! Recibes " + std::to_string(dannio) + " de danno.");
    Pokemon* activo = juga.getPokemonActiv();
    if (activo) {
        activo->dannioRecibido(dannio);
        Logger::getInstancia().log("[Trampa] " + activo->getNombre() + " queda con " + std::to_string(activo->getHpActual()) + " HP.");
    }
}