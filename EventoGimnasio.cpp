//
// Created by Gabri on 5/31/26.
//

#include "EventoGimnasio.h"
#include "Pelea.h"
#include "Logger.h"

EventoGimnasio::EventoGimnasio(const Entrenador& lider) : liderGym(lider) {}

void EventoGimnasio::ejecutar(Jugador& juga) {
    Logger::getInstancia().log("[Gimnasio] ¡Líder " + liderGym.getNombre() + " acepta tu desafío!");
    bool gano = Pelea::empezarBatallaEntrenador(juga, liderGym);
    if (gano) {
        juga.agregarMedalla();
        Logger::getInstancia().log("[Gimnasio] ¡Obtuviste una medalla! Total: " + std::to_string(juga.getMedallas()));
    }
}
