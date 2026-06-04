//
// Created by Gabri on 5/31/26.
//

#include "EventoTiendaObjetos.h"
#include "Logger.h"
#include <iostream>
using namespace std;

EventoTiendaObjetos::EventoTiendaObjetos(const std::vector<Item>& tienda) : tiendaItems(tienda) {}

void EventoTiendaObjetos::ejecutar(Jugador& jugador) {
    Logger& log = Logger::getInstancia();
    cout << "\n[Tienda] Tienes " << jugador.getPlata() << " monedas." << endl;
    cout << "  Articulos disponibles:" << endl;
    for (int i = 0; i < (int)tiendaItems.size(); i++) {
        cout << "    [" << (i+1) << "] " << const_cast<Item&>(tiendaItems[i]).getNombre()
             << " - " << tiendaItems[i].getValor() << " monedas" << endl;
    }
    cout << "    [0] Salir sin comprar" << endl;

    int op = -1;
    while (op < 0 || op > (int)tiendaItems.size()) {
        cout << "  Tu eleccion: ";
        cin >> op;
    }

    if (op == 0) {
        log.log("[Tienda] El jugador salio sin comprar.");
        return;
    }

    Item& item = const_cast<Item&>(tiendaItems[op - 1]);
    if (jugador.gastarPlata(item.getValor())) {
        jugador.getInventario().agregarItem(item);
        cout << "  Compraste: " << item.getNombre() << "!" << endl;
        log.log("[Tienda] Compraste: " + item.getNombre() + " por " + std::to_string(item.getValor()) + " monedas.");
    } else {
        cout << "  No tienes suficientes monedas para eso." << endl;
        log.log("[Tienda] No tenia monedas suficientes.");
    }
}