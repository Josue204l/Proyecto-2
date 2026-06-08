// Evento de gimnasio.
// Acá quise dejarlo simple porque el peso real lo carga la pelea, no tanta logica alrededor.

#include "GymEvent.h"
#include "Battle.h"
#include "Logger.h"
#include "AggressiveStrategy.h"
#include "DefensiveStrategy.h"

GymEvent::GymEvent(const Trainer& lider) : liderGym(lider) {}

void GymEvent::ejecutar(Player& juga) {
    Logger::getInstancia().log("[Gimnasio] ¡Líder " + liderGym.getNombre() + " acepta tu desafío!");
    bool gano;
    if (liderGym.getNombre() == "Misty") {
        DefensiveStrategy estrategia;
        gano = Battle::empezarBatallaEntrenador(juga, liderGym, estrategia);
    } else {
        AggressiveStrategy estrategia;
        gano = Battle::empezarBatallaEntrenador(juga, liderGym, estrategia);
    }
    if (gano) {
        juga.agregarMedalla();
        Logger::getInstancia().log("[Gimnasio] ¡Obtuviste una medalla! Total: " + std::to_string(juga.getMedallas()));
    }
}
