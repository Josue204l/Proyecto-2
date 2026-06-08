//
// Created by Gabri on 5/31/26.
//

#include "TreasureEvent.h"
#include "Logger.h"

TreasureEvent::TreasureEvent(const Item& tesoro) : tesoro(tesoro) {}

void TreasureEvent::ejecutar(Player& jug) {
    Logger::getInstancia().log("[Tesoro] ¡Encontraste un tesoro: " + tesoro.getNombre() + "!");
    jug.getInventario().agregarItem(tesoro);
    Logger::getInstancia().log("[Tesoro] " + tesoro.getNombre() + " agregado a tu inventario.");
}
