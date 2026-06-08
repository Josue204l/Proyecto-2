//
// Created by Gabri on 5/31/26.
//

#include "Pelea.h"
#include "Logger.h"
#include <string>
#include <iostream>
using namespace std;

double Pelea::getMultiplicadorTipo(TipoPokemon atacante, TipoPokemon defensor) {
    // Super efectivo x1.5
    if (atacante == TipoPokemon::FIRE     && defensor == TipoPokemon::GRASS)    return 1.5;
    if (atacante == TipoPokemon::WATER    && defensor == TipoPokemon::FIRE)     return 1.5;
    if (atacante == TipoPokemon::GRASS    && defensor == TipoPokemon::WATER)    return 1.5;
    if (atacante == TipoPokemon::GRASS    && defensor == TipoPokemon::ROCK)     return 1.5;
    if (atacante == TipoPokemon::WATER    && defensor == TipoPokemon::ROCK)     return 1.5;
    if (atacante == TipoPokemon::ELECTRIC && defensor == TipoPokemon::WATER)    return 1.5;
    if (atacante == TipoPokemon::ELECTRIC && defensor == TipoPokemon::FLYING)   return 1.5;
    if (atacante == TipoPokemon::ROCK     && defensor == TipoPokemon::FLYING)   return 1.5;
    if (atacante == TipoPokemon::ROCK     && defensor == TipoPokemon::FIRE)     return 1.5;

    // Poco efectivo x0.5
    if (atacante == TipoPokemon::FIRE   && defensor == TipoPokemon::WATER)      return 0.5;
    if (atacante == TipoPokemon::FIRE   && defensor == TipoPokemon::ROCK)       return 0.5;
    if (atacante == TipoPokemon::GRASS  && defensor == TipoPokemon::FIRE)       return 0.5;
    if (atacante == TipoPokemon::ROCK   && defensor == TipoPokemon::GRASS)      return 0.5;
    if (atacante == TipoPokemon::WATER  && defensor == TipoPokemon::GRASS)      return 0.5;
    if (atacante == TipoPokemon::NORMAL && defensor == TipoPokemon::ROCK)       return 0.5;

    return 1.0;
}

int Pelea::calcularDannio(const Pokemon& atacante, const Pokemon& defensor) {
    double mult = getMultiplicadorTipo(atacante.getTipoPokemon(), defensor.getTipoPokemon());
    // Ataque vs defensa directamente, con el nivel del atacante como factor
    double base = (atacante.getAtaque() - defensor.getDefensa() * 0.5)
                  * (1.0 + atacante.getNivel() * 0.05);
    if (base < 2) base = 2;
    return (int)(base * mult);
}

static void mostrarEstado(Jugador& juga, int idx, const Pokemon& enemigo) {
    cout << "\n--- Estado de batalla ---" << endl;
    cout << "  Tu Pokémon:  " << juga.getPokemon(idx)->getNombre()
         << " HP: " << juga.getPokemon(idx)->getHpActual()
         << "/" << juga.getPokemon(idx)->getHPMAX() << endl;
    cout << "  Enemigo:     " << enemigo.getNombre()
         << " HP: " << enemigo.getHpActual()
         << "/" << enemigo.getHPMAX() << endl;
    cout << "  Ítems en bolsa: " << juga.getInventario().getTamano() << endl;
}

// Devuelve true si usó un ítem, false si no había o canceló
static bool usarItem(Jugador& juga, int idx) {
    // Construir lista solo de ítems que curan
    const auto& items = juga.getInventario().getItems();
    vector<int> indicesCuracion;
    for (int i = 0; i < (int)items.size(); i++) {
        TipoItem t = const_cast<Item&>(items[i]).getTipo();
        if (t == TipoItem::POTION || t == TipoItem::SUPER_POTION || t == TipoItem::RARE_CANDY)
            indicesCuracion.push_back(i);
    }

    if (indicesCuracion.empty()) {
        cout << "  ¡No tienes pociones!" << endl;
        return false;
    }

    cout << "  Ítems disponibles:" << endl;
    for (int i = 0; i < (int)indicesCuracion.size(); i++) {
        Item& item = const_cast<Item&>(items[indicesCuracion[i]]);
        cout << "    [" << (i+1) << "] " << item.getNombre()
             << " (cura " << item.getValor() << " HP)" << endl;
    }

    int op = 0;
    while (op < 1 || op > (int)indicesCuracion.size()) {
        cout << "  Elige (0 cancela): ";
        cin >> op;
        if (op == 0) return false;
    }

    int idxReal = indicesCuracion[op - 1];
    Item item = juga.getInventario().getItem(idxReal);
    juga.getInventario().removerItem(idxReal);
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
        cout << "  ¡No tienes Pokéballs!" << endl;
        return false; // no gastó nada, el turno se repite pero el enemigo sí contraataca
    }
    juga.getInventario().removerItem(idxBall);
    string ball = esGreat ? "Greatball" : "Pokéball";
    cout << "  Lanzaste una " << ball << "..." << endl;

    int chance = esGreat ? 60 : 40;
    if (objetivo.getHpActual() * 4 <= objetivo.getHPMAX())      chance += 30;
    else if (objetivo.getHpActual() * 2 <= objetivo.getHPMAX()) chance += 15;

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
            cout << "  [1] Atacar  [2] Usar ítem  [3] Pokéball  [4] Huir\n  Tu elección: ";
            cin >> accion;
        }

        bool enemigoContraataca = true;

        if (accion == 1) {
            int d = calcularDannio(*juga.getPokemon(idx), pokemonSalvaje);
            pokemonSalvaje.dannioRecibido(d);
            log.log("  " + juga.getPokemon(idx)->getNombre() + " ataca por " + to_string(d) + " de daño.");
            if (pokemonSalvaje.estaMuerto()) {
                victoria = true;
                batallaTerminada = true;
                enemigoContraataca = false;
            }
        } else if (accion == 2) {
            usarItem(juga, idx);
        } else if (accion == 3) {
            bool teniaBall = false;
            const auto& inv = juga.getInventario().getItems();
            for (int i = 0; i < (int)inv.size(); i++) {
                TipoItem t = const_cast<Item&>(inv[i]).getTipo();
                if (t == TipoItem::POKEBALL || t == TipoItem::GREATBALL) { teniaBall = true; break; }
            }
            if (!teniaBall) {
                cout << "  ¡No tienes Pokéballs!" << endl;
            } else if (intentarCaptura(juga, pokemonSalvaje)) {
                batallaTerminada = true;
                enemigoContraataca = false;
            }
        } else if (accion == 4) {
            if (rand() % 100 < 70) {
                log.log("  ¡Escapaste con éxito!");
                cout << "  ¡Escapaste con éxito!" << endl;
                return false;
            } else {
                cout << "  ¡No pudiste escapar!" << endl;
                log.log("  Intentó escapar pero falló.");
            }
        }

        if (batallaTerminada) break;

        // --- Turno del enemigo ---
        if (enemigoContraataca) {
            int d = calcularDannio(pokemonSalvaje, *juga.getPokemon(idx));
            juga.getPokemon(idx)->dannioRecibido(d);
            log.log("  " + pokemonSalvaje.getNombre() + " ataca por " + to_string(d) + " de daño.");

            if (juga.getPokemon(idx)->estaMuerto()) {
                log.log("  ¡" + juga.getPokemon(idx)->getNombre() + " fue debilitado!");
                // busca sustituto
                int nuevoIdx = -1;
                for (int i = 0; i < juga.getTamano(); i++)
                    if (!juga.getPokemon(i)->estaMuerto() && i != idx) { nuevoIdx = i; break; }
                if (nuevoIdx == -1) { batallaTerminada = true; break; }
                cout << "  Elige tu siguiente Pokémon:" << endl;
                for (int i = 0; i < juga.getTamano(); i++)
                    if (!juga.getPokemon(i)->estaMuerto())
                        cout << "    [" << (i+1) << "] " << juga.getPokemon(i)->getNombre()
                             << " HP: " << juga.getPokemon(i)->getHpActual() << endl;
                int sig = 0;
                while (sig < 1 || sig > juga.getTamano() || juga.getPokemon(sig-1)->estaMuerto()) {
                    cout << "  Tu elección: ";
                    cin >> sig;
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
                juga.getPokemon(i)->subirNivelesDirecto(1);
                break;
            }
        }
        return true;  // salimos antes de intentar captura
    }

    // Solo llegamos acá si la batalla terminó sin victoria (captura o huida)
    return false;
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
        while (accion != 1 && accion != 2) {
            cout << "  [1] Atacar  [2] Usar ítem\n  Tu elección: ";
            cin >> accion;
        }

        bool enemigoContraataca = true;

        if (accion == 2) {
            usarItem(juga, idx);
        } else {
            int d = calcularDannio(*juga.getPokemon(idx), enemigo);
            enemigo.dannioRecibido(d);
            log.log("  " + juga.getPokemon(idx)->getNombre() + " ataca por " + to_string(d) + " de daño.");
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
            int nuevoIdx = -1;
            for (int i = 0; i < juga.getTamano(); i++)
                if (!juga.getPokemon(i)->estaMuerto() && i != idx) { nuevoIdx = i; break; }
            if (nuevoIdx == -1) break;
            cout << "  Elige tu siguiente Pokémon:" << endl;
            for (int i = 0; i < juga.getTamano(); i++)
                if (!juga.getPokemon(i)->estaMuerto())
                    cout << "    [" << (i+1) << "] " << juga.getPokemon(i)->getNombre()
                         << " HP: " << juga.getPokemon(i)->getHpActual() << endl;
            int sig = 0;
            while (sig < 1 || sig > juga.getTamano() || juga.getPokemon(sig-1)->estaMuerto()) {
                cout << "  Tu elección: ";
                cin >> sig;
            }
            log.log("  " + juga.getNombre() + " envía a " + juga.getPokemon(sig-1)->getNombre() + "!");
        }
    }

    bool victoria = juga.tienePokemonVivo();
    if (victoria) {
        juga.agregarPlata(entrenador.getPlataGanada());
        log.log("  ¡Victoria! Ganaste " + to_string(entrenador.getPlataGanada()) + " monedas.");
        // Todos los pokemon vivos suben 2 niveles
        for (int i = 0; i < juga.getTamano(); i++) {
            if (!juga.getPokemon(i)->estaMuerto()) {
                juga.getPokemon(i)->subirNivelesDirecto(2);
            }
        }
    } else {
        log.log("  Derrota contra el entrenador " + entrenador.getNombre() + ".");
    }
    return victoria;
}
