//
// Created by Gabri on 5/31/26.
//

#include "EventoTesoro.h"
#include "Logger.h"

EventoTesoro::EventoTesoro(const Item& tesoro) : tesoro(tesoro) {}

void EventoTesoro::ejecutar(Jugador& jug) {
    Logger::getInstancia().log("[Tesoro] ¡Encontraste un tesoro: " + tesoro.getNombre() + "!");
    jug.getInventario().agregarItem(const_cast<Item&>(tesoro));
    Logger::getInstancia().log("[Tesoro] " + tesoro.getNombre() + " agregado a tu inventario.");
}
