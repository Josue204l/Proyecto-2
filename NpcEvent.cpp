//
// Created by Gabri on 5/31/26.
//

#include "NpcEvent.h"
#include "Logger.h"

NpcEvent::NpcEvent(const std::string& mensaje) : mensaje(mensaje) {}

void NpcEvent::ejecutar(Player&) {
    Logger::getInstancia().log("[NPC] " + mensaje);
}
