//
// Created by Gabri on 5/31/26.
//

#include "ShopEvent.h"
#include "Logger.h"
#include <iostream>
using namespace std;

ShopEvent::ShopEvent(const std::vector<Item>& tienda) : tiendaItems(tienda) {}

void ShopEvent::ejecutar(Player& jugador) {
    Logger& log = Logger::getInstancia();
    cout << "\n[Tienda] Tienes " << jugador.getPlata() << " monedas." << endl;
    cout << "  Artículos disponibles:" << endl;
    for (int i = 0; i < (int)tiendaItems.size(); i++) {
        cout << "    [" << (i+1) << "] " << tiendaItems[i].getNombre()
             << " - " << tiendaItems[i].getValor() << " monedas" << endl;
    }
    cout << "    [0] Salir sin comprar" << endl;

    int op = -1;
    while (op < 0 || op > (int)tiendaItems.size()) {
        cout << "  Tu elección: ";
        cin >> op;
    }

    if (op == 0) {
        log.log("[Tienda] El jugador salió sin comprar.");
        return;
    }

    const Item& item = tiendaItems[op - 1];
    if (jugador.gastarPlata(item.getValor())) {
        jugador.getInventario().agregarItem(item);
        cout << "  ¡Compraste: " << item.getNombre() << "!" << endl;
        log.log("[Tienda] Compraste: " + item.getNombre() + " por " + std::to_string(item.getValor()) + " monedas.");
    } else {
        cout << "  No tienes suficientes monedas para eso." << endl;
        log.log("[Tienda] No tenía monedas suficientes.");
    }
}
