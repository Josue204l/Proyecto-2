//
// Created by Gabri on 5/31/26.
//

#include "Pelea.h"
#include "Logger.h"
#include <string>
#include <iostream>
using namespace std;

double Pelea::getMultiplicadorTipo(TipoPokemon atacante, TipoPokemon defensor) {
    // Fire > Grass, Water > Fire, Grass > Water, Electric > Water/Flying, Rock > Flying
    if (atacante == TipoPokemon::FIRE   && defensor == TipoPokemon::GRASS)    return 2.0;
    if (atacante == TipoPokemon::WATER  && defensor == TipoPokemon::FIRE)     return 2.0;
    if (atacante == TipoPokemon::GRASS  && defensor == TipoPokemon::WATER)    return 2.0;
    if (atacante == TipoPokemon::ELECTRIC && defensor == TipoPokemon::WATER)  return 2.0;
    if (atacante == TipoPokemon::ELECTRIC && defensor == TipoPokemon::FLYING) return 2.0;
    if (atacante == TipoPokemon::ROCK   && defensor == TipoPokemon::FLYING)   return 2.0;
    if (atacante == TipoPokemon::FIRE   && defensor == TipoPokemon::WATER)    return 0.5;
    if (atacante == TipoPokemon::GRASS  && defensor == TipoPokemon::FIRE)     return 0.5;
    if (atacante == TipoPokemon::WATER  && defensor == TipoPokemon::GRASS)    return 0.5;
    return 1.0;
}

int Pelea::calcularDannio(const Pokemon& atacante, const Pokemon& defensor) {
    double mult = getMultiplicadorTipo(atacante.getTipoPokemon(), defensor.getTipoPokemon());
    int base = atacante.getAtaque() - (defensor.getDefensa() / 2);
    if (base < 1) base = 1;
    return (int)(base * mult);
}

// Muestra el estado actual de la batalla en consola
static void mostrarEstadoBatalla(Jugador& juga, int activoIdx, const Pokemon& enemigo) {
    cout << "\n--- Estado de batalla ---" << endl;
    cout << "  Tu Pokemon:  " << juga.getPokemon(activoIdx)->getNombre()
         << " HP: " << juga.getPokemon(activoIdx)->getHpActual()
         << "/" << juga.getPokemon(activoIdx)->getHPMAX() << endl;
    cout << "  Enemigo:     " << enemigo.getNombre()
         << " HP: " << enemigo.getHpActual()
         << "/" << enemigo.getHPMAX() << endl;
    cout << "  Items en bolsa: " << juga.getInventario().getTamano() << endl;
}

// Pide al jugador que elija un item del inventario; retorna true si uso uno
static bool usarItemEnBatalla(Jugador& juga, int activoIdx) {
    Logger& log = Logger::getInstancia();
    if (juga.getInventario().estaVacio()) {
        cout << "  No tienes items!" << endl;
        return false;
    }
    cout << "  Items disponibles:" << endl;
    const auto& items = juga.getInventario().getItems();
    for (int i = 0; i < (int)items.size(); i++) {
        cout << "    [" << (i+1) << "] " << const_cast<Item&>(items[i]).getNombre()
             << " (valor: " << const_cast<Item&>(items[i]).getValor() << ")" << endl;
    }
    int op = 0;
    while (op < 1 || op > (int)items.size()) {
        cout << "  Elige item (0 para cancelar): ";
        cin >> op;
        if (op == 0) return false;
    }
    Item item = juga.getInventario().getItem(op - 1);
    juga.getInventario().removerItem(op - 1);
    // Aplica efecto: si es POTION cura, sino da daño al enemigo (simplificado)
    int hp = juga.getPokemon(activoIdx)->getHpActual() + item.getValor();
    int max = juga.getPokemon(activoIdx)->getHPMAX();
    // Limitar al maximo
    juga.getPokemon(activoIdx)->dannioRecibido(-(min(item.getValor(), max - juga.getPokemon(activoIdx)->getHpActual())));
    log.log("  Usaste " + item.getNombre() + " en " + juga.getPokemon(activoIdx)->getNombre() + ".");
    cout << "  " << juga.getPokemon(activoIdx)->getNombre() << " recupero HP!" << endl;
    return true;
}

bool Pelea::empezarBatallaSalvaje(Jugador& juga, Pokemon pokemonSalvaje) {
    Logger& log = Logger::getInstancia();

    // Busca el indice del primer pokemon vivo
    int activoIdx = -1;
    for (int i = 0; i < juga.getTamano(); i++) {
        if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
    }
    if (activoIdx == -1) return false;

    cout << "\n[Batalla] " << juga.getPokemon(activoIdx)->getNombre()
         << " vs " << pokemonSalvaje.getNombre() << " salvaje (Nivel " << pokemonSalvaje.getNivel() << ")" << endl;
    log.log("  [Batalla] " + juga.getPokemon(activoIdx)->getNombre() + " vs " + pokemonSalvaje.getNombre() + " (salvaje)");

    while (true) {
        mostrarEstadoBatalla(juga, activoIdx, pokemonSalvaje);

        // Pregunta al jugador que hacer
        int accion = 0;
        while (accion != 1 && accion != 2) {
            cout << "  [1] Atacar  [2] Usar item\n  Tu eleccion: ";
            cin >> accion;
        }

        if (accion == 2 && usarItemEnBatalla(juga, activoIdx)) {
            // turno de item: el enemigo igual ataca
        } else {
            // Turno del jugador: ataca
            int dannoJugador = calcularDannio(*juga.getPokemon(activoIdx), pokemonSalvaje);
            pokemonSalvaje.dannioRecibido(dannoJugador);
            cout << "  " << juga.getPokemon(activoIdx)->getNombre() << " ataca por " << dannoJugador << " de danno." << endl;
            log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " ataca por " + std::to_string(dannoJugador) + " de danno.");
            if (pokemonSalvaje.estaMuerto()) break;
        }

        // Turno del enemigo: contraataca
        int dannoEnemigo = calcularDannio(pokemonSalvaje, *juga.getPokemon(activoIdx));
        juga.getPokemon(activoIdx)->dannioRecibido(dannoEnemigo);
        cout << "  " << pokemonSalvaje.getNombre() << " contraataca por " << dannoEnemigo << " de danno." << endl;
        log.log("  " + pokemonSalvaje.getNombre() + " contraataca por " + std::to_string(dannoEnemigo) + " de danno.");

        // Si el pokemon activo cayo, pregunta si cambiar
        if (juga.getPokemon(activoIdx)->estaMuerto()) {
            cout << "  " << juga.getPokemon(activoIdx)->getNombre() << " fue debilitado!" << endl;
            // Muestra pokemon vivos disponibles
            bool haySustituto = false;
            for (int i = 0; i < juga.getTamano(); i++) {
                if (!juga.getPokemon(i)->estaMuerto() && i != activoIdx) { haySustituto = true; break; }
            }
            if (!haySustituto) break;
            cout << "  Elige tu siguiente Pokemon:" << endl;
            for (int i = 0; i < juga.getTamano(); i++) {
                if (!juga.getPokemon(i)->estaMuerto())
                    cout << "    [" << (i+1) << "] " << juga.getPokemon(i)->getNombre()
                         << " HP: " << juga.getPokemon(i)->getHpActual() << endl;
            }
            int sig = 0;
            while (sig < 1 || sig > juga.getTamano() || juga.getPokemon(sig-1)->estaMuerto()) {
                cout << "  Tu eleccion: ";
                cin >> sig;
            }
            activoIdx = sig - 1;
            log.log("  " + juga.getNombre() + " envia a " + juga.getPokemon(activoIdx)->getNombre() + "!");
            cout << "  Vas con " << juga.getPokemon(activoIdx)->getNombre() << "!" << endl;
        }
    }

    bool victoria = pokemonSalvaje.estaMuerto();
    if (victoria) {
        log.log("  Resultado: Victoria! " + pokemonSalvaje.getNombre() + " fue derrotado.");
        // busca de nuevo el activo por indice tras posible realloc
        for (int i = 0; i < juga.getTamano(); i++) {
            if (!juga.getPokemon(i)->estaMuerto()) {
                int exp = pokemonSalvaje.getNivel() * 50;
                juga.getPokemon(i)->ganarExperiencia(exp);
                log.log("  " + juga.getPokemon(i)->getNombre() + " gano " + std::to_string(exp) + " de experiencia.");
                if (juga.getPokemon(i)->puedeEvolucionar())
                    log.log("  " + juga.getPokemon(i)->getNombre() + " puede evolucionar!");
                break;
            }
        }
    } else {
        log.log("  Resultado: Derrota. El jugador perdio la batalla.");
    }
    return victoria;
}

bool Pelea::empezarBatallaEntrenador(Jugador& juga, Entrenador entrenador) {
    Logger& log = Logger::getInstancia();
    cout << "\n[Batalla vs Entrenador] " << juga.getNombre() << " vs " << entrenador.getNombre() << endl;
    log.log("  [Batalla vs Entrenador] " + juga.getNombre() + " vs " + entrenador.getNombre());

    int enemigoIdx = 0;

    while (enemigoIdx < (int)entrenador.getEquipo().size()) {
        // Busca el pokemon activo del jugador
        int activoIdx = -1;
        for (int i = 0; i < juga.getTamano(); i++) {
            if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
        }
        if (activoIdx == -1) break;

        // Avanza al siguiente enemigo vivo
        while (enemigoIdx < (int)entrenador.getEquipo().size() &&
               entrenador.getPokemon(enemigoIdx).estaMuerto()) {
            enemigoIdx++;
        }
        if (enemigoIdx >= (int)entrenador.getEquipo().size()) break;

        Pokemon& enemigo = entrenador.getPokemon(enemigoIdx);
        mostrarEstadoBatalla(juga, activoIdx, enemigo);

        int accion = 0;
        while (accion != 1 && accion != 2) {
            cout << "  [1] Atacar  [2] Usar item\n  Tu eleccion: ";
            cin >> accion;
        }

        if (accion == 2 && usarItemEnBatalla(juga, activoIdx)) {
            // solo el enemigo ataca este turno
        } else {
            int dannoJugador = calcularDannio(*juga.getPokemon(activoIdx), enemigo);
            enemigo.dannioRecibido(dannoJugador);
            cout << "  " << juga.getPokemon(activoIdx)->getNombre() << " ataca por " << dannoJugador << " de danno." << endl;
            log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " ataca por " + std::to_string(dannoJugador) + " de danno.");
            if (enemigo.estaMuerto()) {
                int exp = enemigo.getNivel() * 50;
                juga.getPokemon(activoIdx)->ganarExperiencia(exp);
                log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " gano " + std::to_string(exp) + " exp.");
                enemigoIdx++;
                continue;
            }
        }

        // Turno del enemigo: contraataca
        int dannoEnemigo = calcularDannio(enemigo, *juga.getPokemon(activoIdx));
        juga.getPokemon(activoIdx)->dannioRecibido(dannoEnemigo);
        cout << "  " << enemigo.getNombre() << " contraataca por " << dannoEnemigo << " de danno." << endl;
        log.log("  " + enemigo.getNombre() + " contraataca por " + std::to_string(dannoEnemigo) + " de danno.");

        if (juga.getPokemon(activoIdx)->estaMuerto()) {
            cout << "  " << juga.getPokemon(activoIdx)->getNombre() << " fue debilitado!" << endl;
            bool haySustituto = false;
            for (int i = 0; i < juga.getTamano(); i++) {
                if (!juga.getPokemon(i)->estaMuerto() && i != activoIdx) { haySustituto = true; break; }
            }
            if (!haySustituto) break;
            cout << "  Elige tu siguiente Pokemon:" << endl;
            for (int i = 0; i < juga.getTamano(); i++) {
                if (!juga.getPokemon(i)->estaMuerto())
                    cout << "    [" << (i+1) << "] " << juga.getPokemon(i)->getNombre()
                         << " HP: " << juga.getPokemon(i)->getHpActual() << endl;
            }
            int sig = 0;
            while (sig < 1 || sig > juga.getTamano() || juga.getPokemon(sig-1)->estaMuerto()) {
                cout << "  Tu eleccion: ";
                cin >> sig;
            }
            log.log("  " + juga.getNombre() + " envia a " + juga.getPokemon(sig-1)->getNombre() + "!");
            cout << "  Vas con " << juga.getPokemon(sig-1)->getNombre() << "!" << endl;
        }
    }

    bool victoria = juga.tienePokemonVivo();
    if (victoria) {
        juga.agregarPlata(entrenador.getPlataGanada());
        log.log("  Victoria! Ganaste " + std::to_string(entrenador.getPlataGanada()) + " monedas.");
    } else {
        log.log("  Derrota contra el entrenador " + entrenador.getNombre() + ".");
    }
    return victoria;
}