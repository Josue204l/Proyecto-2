//
// Created by Gabri on 5/31/26.
//

#include "Pelea.h"
#include "Logger.h"
#include <string>
#include <iostream>
using namespace std;

double Pelea::getMultiplicadorTipo(TipoPokemon atacante, TipoPokemon defensor) {
    if (atacante == TipoPokemon::FIRE   && defensor == TipoPokemon::GRASS)    return 1.5;
    if (atacante == TipoPokemon::WATER  && defensor == TipoPokemon::FIRE)     return 1.5;
    if (atacante == TipoPokemon::GRASS  && defensor == TipoPokemon::WATER)    return 1.5;
    if (atacante == TipoPokemon::ELECTRIC && defensor == TipoPokemon::WATER)  return 1.5;
    if (atacante == TipoPokemon::ELECTRIC && defensor == TipoPokemon::FLYING) return 1.5;
    if (atacante == TipoPokemon::ROCK   && defensor == TipoPokemon::FLYING)   return 1.5;
    if (atacante == TipoPokemon::FIRE   && defensor == TipoPokemon::WATER)    return 0.75;
    if (atacante == TipoPokemon::GRASS  && defensor == TipoPokemon::FIRE)     return 0.75;
    if (atacante == TipoPokemon::WATER  && defensor == TipoPokemon::GRASS)    return 0.75;
    return 1.0;
}

int Pelea::calcularDannio(const Pokemon& atacante, const Pokemon& defensor) {
    double mult = getMultiplicadorTipo(atacante.getTipoPokemon(), defensor.getTipoPokemon());
    int base = atacante.getAtaque() - (defensor.getDefensa() / 4);
    if (base < 3) base = 3;
    return (int)(base * mult);
}

static void mostrarEstado(Jugador& juga, int idx, const Pokemon& enemigo) {
    cout << "\n=== Estado de batalla ===" << endl;
    cout << "  Tu Pokémon:  " << juga.getPokemon(idx)->getNombre()
         << " (Nivel " << juga.getPokemon(idx)->getNivel() << ")" << endl;
    cout << "  HP: " << juga.getPokemon(idx)->getHpActual()
         << "/" << juga.getPokemon(idx)->getHPMAX() << endl;
    cout << "  ATK: " << juga.getPokemon(idx)->getAtaque()
         << "  DEF: " << juga.getPokemon(idx)->getDefensa() << endl;
    cout << "  Enemigo:     " << enemigo.getNombre()
         << " (Nivel " << enemigo.getNivel() << ")" << endl;
    cout << "  HP: " << enemigo.getHpActual()
         << "/" << enemigo.getHPMAX() << endl;
    cout << "  Ítems en bolsa: " << juga.getInventario().getTamano() << endl;
    cout << "============================" << endl;
}

// Devuelve true si usó un ítem, false si no había o canceló
static bool usarItem(Jugador& juga, int idx) {
    if (juga.getInventario().estaVacio()) {
        cout << "  [!] No tienes ítems en tu inventario." << endl;
        return false;
    }
    const auto& items = juga.getInventario().getItems();
    cout << "\n  --- Ítems disponibles ---" << endl;
    for (int i = 0; i < (int)items.size(); i++)
        cout << "    [" << (i+1) << "] " << const_cast<Item&>(items[i]).getNombre()
             << " (" << const_cast<Item&>(items[i]).getValor() << " HP)" << endl;
    cout << "    [0] Cancelar" << endl;
    int op = -1;
    while (op < 0 || op > (int)items.size()) {
        cout << "  Elige: ";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            op = -1;
        }
        if (op == 0) {
            cout << "  Cancelado." << endl;
            return false;
        }
    }
    Item item = juga.getInventario().getItem(op - 1);
    juga.getInventario().removerItem(op - 1);
    int cura = min(item.getValor(), juga.getPokemon(idx)->getHPMAX() - juga.getPokemon(idx)->getHpActual());
    juga.getPokemon(idx)->dannioRecibido(-cura);
    Logger::getInstancia().log("  Usaste " + item.getNombre() + " en " + juga.getPokemon(idx)->getNombre() + ".");
    cout << "  " << juga.getPokemon(idx)->getNombre() << " recuperó " << cura << " HP!" << endl;
    return true;
}

// Devuelve true si la captura terminó la batalla (éxito o fallo sin contraataque deseado)
static bool intentarCaptura(Jugador& juga, Pokemon& objetivo) {
    const auto& items = juga.getInventario().getItems();
    int idxBall = -1;
    bool esGreat = false;
    for (int i = 0; i < (int)items.size(); i++) {
        TipoItem t = const_cast<Item&>(items[i]).getTipo();
        if (t == TipoItem::GREATBALL) { idxBall = i; esGreat = true; break; }
        if (t == TipoItem::POKEBALL && idxBall == -1) idxBall = i;
    }
    if (idxBall == -1) {
        cout << "  [!] No tienes Pokéballs en tu inventario." << endl;
        return false; // no gastó nada, el turno se repite pero el enemigo sí contraataca
    }
    juga.getInventario().removerItem(idxBall);
    string ball = esGreat ? "Greatball" : "Pokéball";
    cout << "  Lanzaste una " << ball << "..." << endl;

    int chance = esGreat ? 60 : 40;
    if (objetivo.getHpActual() * 4 <= objetivo.getHPMAX())      chance += 30;
    else if (objetivo.getHpActual() * 2 <= objetivo.getHPMAX()) chance += 15;

    cout << "  Probabilidad de captura: " << chance << "%" << endl;
    if ((rand() % 100) < chance) {
        if (juga.getTamano() < 6) {
            juga.agregarPokemon(objetivo);
            cout << "  ¡Atrapaste a " << objetivo.getNombre() << "!" << endl;
        } else {
            cout << "  ¡Atrapaste a " << objetivo.getNombre() << " pero el equipo está lleno!" << endl;
        }
        Logger::getInstancia().log("  ¡Capturaste a " + objetivo.getNombre() + "!");
        return true; // batalla termina
    }
    cout << "  ¡" << objetivo.getNombre() << " escapó!" << endl;
    Logger::getInstancia().log("  " + objetivo.getNombre() + " escapó.");
    return false; // el enemigo contraataca este turno
}

bool Pelea::empezarBatallaSalvaje(Jugador& juga, Pokemon pokemonSalvaje) {
    Logger& log = Logger::getInstancia();

    int idx = -1;
    for (int i = 0; i < juga.getTamano(); i++)
        if (!juga.getPokemon(i)->estaMuerto()) { idx = i; break; }
    if (idx == -1) return false;

    cout << "\n[Batalla] " << juga.getPokemon(idx)->getNombre()
         << " vs " << pokemonSalvaje.getNombre()
         << " salvaje (Nivel " << pokemonSalvaje.getNivel() << ")" << endl;

    bool batallaTerminada = false;
    bool victoria = false;

    while (!batallaTerminada) {
        mostrarEstado(juga, idx, pokemonSalvaje);

        // --- Turno del jugador ---
        int accion = 0;
        while (accion < 1 || accion > 4) {
            cout << "\n  ¿Qué quieres hacer?" << endl;
            cout << "    [1] Atacar" << endl;
            cout << "    [2] Usar ítem" << endl;
            cout << "    [3] Lanzar Pokéball" << endl;
            cout << "    [4] Cambiar Pokémon" << endl;
            cout << "  Tu elección: ";
            cin >> accion;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                accion = 0;
            }
        }

        bool enemigoContraataca = true;

        if (accion == 1) {
            int d = calcularDannio(*juga.getPokemon(idx), pokemonSalvaje);
            pokemonSalvaje.dannioRecibido(d);
            log.log("  " + juga.getPokemon(idx)->getNombre() + " ataca por " + to_string(d) + " de daño.");
            cout << "  ¡Caused " << d << " damage!" << endl;
            if (pokemonSalvaje.estaMuerto()) {
                victoria = true;
                batallaTerminada = true;
                enemigoContraataca = false;
            }
        } else if (accion == 2) {
            usarItem(juga, idx); // si no hay ítem el jugador pierde su turno pero el enemigo ataca
        } else if (accion == 3) {
            bool teniaBall = false;
            const auto& inv = juga.getInventario().getItems();
            for (int i = 0; i < (int)inv.size(); i++) {
                TipoItem t = const_cast<Item&>(inv[i]).getTipo();
                if (t == TipoItem::POKEBALL || t == TipoItem::GREATBALL) { teniaBall = true; break; }
            }
            if (!teniaBall) {
                cout << "  [!] No tienes Pokéballs. Pierdes tu turno." << endl;
                // sin ball: pierde el turno pero el enemigo ataca UNA sola vez
            } else if (intentarCaptura(juga, pokemonSalvaje)) {
                batallaTerminada = true;
                enemigoContraataca = false;
            }
            // si falló la captura, el enemigo contraataca una vez
        } else if (accion == 4) {
            // Cambiar Pokémon
            cout << "\n  --- Tu equipo ---" << endl;
            for (int i = 0; i < juga.getTamano(); i++) {
                string estado = juga.getPokemon(i)->estaMuerto() ? " [DEBILITADO]" : "";
                cout << "    [" << (i+1) << "] " << juga.getPokemon(i)->getNombre()
                     << " HP: " << juga.getPokemon(i)->getHpActual()
                     << "/" << juga.getPokemon(i)->getHPMAX() << estado << endl;
            }
            int nuevoIdx = -1;
            while (nuevoIdx < 1 || nuevoIdx > juga.getTamano() || juga.getPokemon(nuevoIdx-1)->estaMuerto() || nuevoIdx-1 == idx) {
                cout << "  Elige Pokémon (debe estar vivo y no ser el actual): ";
                cin >> nuevoIdx;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    nuevoIdx = -1;
                }
                if (nuevoIdx == idx) {
                    cout << "  [!] Ese Pokémon ya está activo." << endl;
                }
                if (nuevoIdx >= 1 && nuevoIdx <= juga.getTamano() && juga.getPokemon(nuevoIdx-1)->estaMuerto()) {
                    cout << "  [!] Ese Pokémon está debilitado." << endl;
                }
            }
            idx = nuevoIdx - 1;
            cout << "  ¡Vas con " << juga.getPokemon(idx)->getNombre() << "!" << endl;
            enemigoContraataca = false; // cambiar Pokémon no gana turno del enemigo
        }

        if (batallaTerminada) break;

        // --- Turno del enemigo ---
        if (enemigoContraataca) {
            int d = calcularDannio(pokemonSalvaje, *juga.getPokemon(idx));
            juga.getPokemon(idx)->dannioRecibido(d);
            log.log("  " + pokemonSalvaje.getNombre() + " ataca por " + to_string(d) + " de daño.");

            if (juga.getPokemon(idx)->estaMuerto()) {
                log.log("  ¡" + juga.getPokemon(idx)->getNombre() + " fue debilitado!");
                cout << "\n  [!] " << juga.getPokemon(idx)->getNombre() << " ha sido debilitado!" << endl;
                // busca sustituto
                int nuevoIdx = -1;
                for (int i = 0; i < juga.getTamano(); i++)
                    if (!juga.getPokemon(i)->estaMuerto() && i != idx) { nuevoIdx = i; break; }
                if (nuevoIdx == -1) { batallaTerminada = true; break; }
                cout << "\n  Elige tu siguiente Pokémon:" << endl;
                for (int i = 0; i < juga.getTamano(); i++)
                    if (!juga.getPokemon(i)->estaMuerto())
                        cout << "    [" << (i+1) << "] " << juga.getPokemon(i)->getNombre()
                             << " HP: " << juga.getPokemon(i)->getHpActual()
                             << "/" << juga.getPokemon(i)->getHPMAX() << endl;
                int sig = 0;
                while (sig < 1 || sig > juga.getTamano() || juga.getPokemon(sig-1)->estaMuerto()) {
                    cout << "  Tu elección: ";
                    cin >> sig;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        sig = 0;
                    }
                }
                idx = sig - 1;
                cout << "  ¡Vas con " << juga.getPokemon(idx)->getNombre() << "!" << endl;
            }
        }
    }

    if (victoria) {
        log.log("  Resultado: ¡Victoria! " + pokemonSalvaje.getNombre() + " fue derrotado.");
        for (int i = 0; i < juga.getTamano(); i++) {
            if (!juga.getPokemon(i)->estaMuerto()) {
                int exp = pokemonSalvaje.getNivel() * 50;
                juga.getPokemon(i)->ganarExperiencia(exp);
                log.log("  " + juga.getPokemon(i)->getNombre() + " ganó " + to_string(exp) + " de experiencia.");
                if (juga.getPokemon(i)->puedeEvolucionar()) {
                    log.log("  ¡" + juga.getPokemon(i)->getNombre() + " puede evolucionar!");
                }
                break;
            }
        }
    }
    return victoria;
}

bool Pelea::empezarBatallaEntrenador(Jugador& juga, Entrenador entrenador) {
    Logger& log = Logger::getInstancia();
    cout << "\n[Batalla vs Entrenador] " << juga.getNombre() << " vs " << entrenador.getNombre() << endl;
    log.log("  [Batalla vs Entrenador] " + juga.getNombre() + " vs " + entrenador.getNombre());

    int enemigoIdx = 0;

    while (enemigoIdx < (int)entrenador.getEquipo().size()) {
        // jugador activo
        int idx = -1;
        for (int i = 0; i < juga.getTamano(); i++)
            if (!juga.getPokemon(i)->estaMuerto()) { idx = i; break; }
        if (idx == -1) break;

        // enemigo activo
        while (enemigoIdx < (int)entrenador.getEquipo().size() &&
               entrenador.getPokemon(enemigoIdx).estaMuerto())
            enemigoIdx++;
        if (enemigoIdx >= (int)entrenador.getEquipo().size()) break;

        Pokemon& enemigo = entrenador.getPokemon(enemigoIdx);
        mostrarEstado(juga, idx, enemigo);

        int accion = 0;
        while (accion < 1 || accion > 3) {
            cout << "\n  ¿Qué quieres hacer?" << endl;
            cout << "    [1] Atacar" << endl;
            cout << "    [2] Usar ítem" << endl;
            cout << "    [3] Cambiar Pokémon" << endl;
            cout << "  Tu elección: ";
            cin >> accion;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                accion = 0;
            }
        }

        bool enemigoContraataca = true;

        if (accion == 2) {
            usarItem(juga, idx);
        } else if (accion == 3) {
            // Cambiar Pokémon
            cout << "\n  --- Tu equipo ---" << endl;
            for (int i = 0; i < juga.getTamano(); i++) {
                string estado = juga.getPokemon(i)->estaMuerto() ? " [DEBILITADO]" : "";
                cout << "    [" << (i+1) << "] " << juga.getPokemon(i)->getNombre()
                     << " HP: " << juga.getPokemon(i)->getHpActual()
                     << "/" << juga.getPokemon(i)->getHPMAX() << estado << endl;
            }
            int nuevoIdx = -1;
            while (nuevoIdx < 1 || nuevoIdx > juga.getTamano() || juga.getPokemon(nuevoIdx-1)->estaMuerto() || nuevoIdx-1 == idx) {
                cout << "  Elige Pokémon (debe estar vivo y no ser el actual): ";
                cin >> nuevoIdx;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    nuevoIdx = -1;
                }
                if (nuevoIdx == idx) {
                    cout << "  [!] Ese Pokémon ya está activo." << endl;
                }
                if (nuevoIdx >= 1 && nuevoIdx <= juga.getTamano() && juga.getPokemon(nuevoIdx-1)->estaMuerto()) {
                    cout << "  [!] Ese Pokémon está debilitado." << endl;
                }
            }
            idx = nuevoIdx - 1;
            cout << "  ¡Vas con " << juga.getPokemon(idx)->getNombre() << "!" << endl;
            enemigoContraataca = false; // cambiar Pokémon no gana turno del enemigo
        } else {
            int d = calcularDannio(*juga.getPokemon(idx), enemigo);
            enemigo.dannioRecibido(d);
            log.log("  " + juga.getPokemon(idx)->getNombre() + " ataca por " + to_string(d) + " de daño.");
            cout << "  ¡Caused " << d << " damage!" << endl;
            if (enemigo.estaMuerto()) {
                int exp = enemigo.getNivel() * 50;
                juga.getPokemon(idx)->ganarExperiencia(exp);
                log.log("  " + juga.getPokemon(idx)->getNombre() + " ganó " + to_string(exp) + " exp.");
                if (juga.getPokemon(idx)->puedeEvolucionar()) {
                    log.log("  ¡" + juga.getPokemon(idx)->getNombre() + " puede evolucionar!");
                }
                enemigoIdx++;
                enemigoContraataca = false;
            }
        }

        if (!enemigoContraataca) continue;

        int d = calcularDannio(enemigo, *juga.getPokemon(idx));
        juga.getPokemon(idx)->dannioRecibido(d);
        log.log("  " + enemigo.getNombre() + " ataca por " + to_string(d) + " de daño.");

        if (juga.getPokemon(idx)->estaMuerto()) {
            log.log("  ¡" + juga.getPokemon(idx)->getNombre() + " fue debilitado!");
            cout << "\n  [!] " << juga.getPokemon(idx)->getNombre() << " ha sido debilitado!" << endl;
            int nuevoIdx = -1;
            for (int i = 0; i < juga.getTamano(); i++)
                if (!juga.getPokemon(i)->estaMuerto() && i != idx) { nuevoIdx = i; break; }
            if (nuevoIdx == -1) break;
            cout << "\n  Elige tu siguiente Pokémon:" << endl;
            for (int i = 0; i < juga.getTamano(); i++)
                if (!juga.getPokemon(i)->estaMuerto())
                    cout << "    [" << (i+1) << "] " << juga.getPokemon(i)->getNombre()
                         << " HP: " << juga.getPokemon(i)->getHpActual()
                         << "/" << juga.getPokemon(i)->getHPMAX() << endl;
            int sig = 0;
            while (sig < 1 || sig > juga.getTamano() || juga.getPokemon(sig-1)->estaMuerto()) {
                cout << "  Tu elección: ";
                cin >> sig;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    sig = 0;
                }
            }
            idx = sig - 1;
            cout << "  ¡Vas con " << juga.getPokemon(idx)->getNombre() << "!" << endl;
            log.log("  " + juga.getNombre() + " envía a " + juga.getPokemon(idx)->getNombre() + "!");
        }
    }

    bool victoria = juga.tienePokemonVivo();
    if (victoria) {
        juga.agregarPlata(entrenador.getPlataGanada());
        log.log("  ¡Victoria! Ganaste " + to_string(entrenador.getPlataGanada()) + " monedas.");
    } else {
        log.log("  Derrota contra el entrenador " + entrenador.getNombre() + ".");
    }
    return victoria;
}
