//
// Created by Gabri on 5/31/26.
//

#include "EventoTiendaObjetos.h"
#include "Logger.h"

EventoTiendaObjetos::EventoTiendaObjetos(const std::vector<Item>& tienda) : tiendaItems(tienda) {}

void EventoTiendaObjetos::ejecutar(Jugador& jugador) {
    Logger& log = Logger::getInstancia();
    log.log("[Tienda] Bienvenido! Tienes " + std::to_string(jugador.getPlata()) + " monedas.");
    // Auto-buy: purchase first affordable item
    for (auto& item : tiendaItems) {
        if (jugador.gastarPlata(item.getValor())) {
            jugador.getInventario().agregarItem(const_cast<Item&>(item));
            log.log("[Tienda] Compraste: " + item.getNombre() + " por " + std::to_string(item.getValor()) + " monedas.");
            break;
        }
    }
}