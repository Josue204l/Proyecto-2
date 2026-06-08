//
// Created by Gabri on 5/31/26.
//

#include "TrainerBattleEvent.h"
#include "Battle.h"
#include "Logger.h"
#include "RandomStrategy.h"

TrainerBattleEvent::TrainerBattleEvent(const Trainer& ent) : entrenador(ent) {}

void TrainerBattleEvent::ejecutar(Player& jug) {
    Logger::getInstancia().log("[Evento] Entrenador " + entrenador.getNombre() + " ¡quiere batallar!");
    RandomStrategy estrategia;
    Battle::empezarBatallaEntrenador(jug, entrenador, estrategia);
}
