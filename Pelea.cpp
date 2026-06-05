#include "Pelea.h"
#include "Logger.h"
#include <string>
#include <iostream>
using namespace std;

double Pelea::getMultiplicadorTipo(TipoPokemon atacante, TipoPokemon defensor) {
    if (atacante == TipoPokemon::FIRE    && defensor == TipoPokemon::GRASS)    return 2.0;
    if (atacante == TipoPokemon::WATER   && defensor == TipoPokemon::FIRE)     return 2.0;
    if (atacante == TipoPokemon::GRASS   && defensor == TipoPokemon::WATER)    return 2.0;
    if (atacante == TipoPokemon::ELECTRIC && defensor == TipoPokemon::WATER)   return 2.0;
    if (atacante == TipoPokemon::ELECTRIC && defensor == TipoPokemon::FLYING)  return 2.0;
    if (atacante == TipoPokemon::ROCK    && defensor == TipoPokemon::FLYING)   return 2.0;
    if (atacante == TipoPokemon::FIRE    && defensor == TipoPokemon::WATER)    return 0.5;
    if (atacante == TipoPokemon::GRASS   && defensor == TipoPokemon::FIRE)     return 0.5;
    if (atacante == TipoPokemon::WATER   && defensor == TipoPokemon::GRASS)    return 0.5;
    if (atacante == TipoPokemon::GRASS   && defensor == TipoPokemon::ROCK)     return 0.5;
    if (atacante == TipoPokemon::ROCK    && defensor == TipoPokemon::WATER)    return 0.5;
    return 1.0;
}

int Pelea::calcularDannio(const Pokemon& atacante, const Pokemon& defensor) {
    double mult = getMultiplicadorTipo(atacante.getTipoPokemon(), defensor.getTipoPokemon());
    // Damage formula balanced so fights last several turns at equal level
    // Balanced formula: fights last 5-8 turns at similar levels
    int base = atacante.getAtaque() / 4 - defensor.getDefensa() / 8;
    if (base < 2) base = 2;
    int dannio = (int)(base * mult);
    // Small random variance +-15%
    int varianza = max(1, dannio / 6);
    dannio = dannio - varianza + (rand() % (varianza * 2 + 1));
    if (dannio < 1) dannio = 1;
    return dannio;
}

// Fully automatic wild battle — no cin
bool Pelea::empezarBatallaSalvaje(Jugador& juga, Pokemon pokemonSalvaje) {
    Logger& log = Logger::getInstancia();

    int activoIdx = -1;
    for (int i = 0; i < juga.getTamano(); i++) {
        if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
    }
    if (activoIdx == -1) return false;

    log.log("[Batalla] " + juga.getPokemon(activoIdx)->getNombre()
            + " (Nv." + to_string(juga.getPokemon(activoIdx)->getNivel()) + ")"
            + " vs " + pokemonSalvaje.getNombre()
            + " (Nv." + to_string(pokemonSalvaje.getNivel()) + ")");

    int turno = 0;
    while (!pokemonSalvaje.estaMuerto()) {
        turno++;

        // Player attacks
        int dannoJugador = calcularDannio(*juga.getPokemon(activoIdx), pokemonSalvaje);
        pokemonSalvaje.dannioRecibido(dannoJugador);
        log.log("  Turno " + to_string(turno) + ": "
                + juga.getPokemon(activoIdx)->getNombre()
                + " ataca por " + to_string(dannoJugador) + " danno."
                + " (rival HP: " + to_string(pokemonSalvaje.getHpActual()) + "/" + to_string(pokemonSalvaje.getHPMAX()) + ")");

        if (pokemonSalvaje.estaMuerto()) break;

        // Enemy attacks
        int dannoEnemigo = calcularDannio(pokemonSalvaje, *juga.getPokemon(activoIdx));
        juga.getPokemon(activoIdx)->dannioRecibido(dannoEnemigo);
        log.log("  Turno " + to_string(turno) + ": "
                + pokemonSalvaje.getNombre()
                + " contraataca por " + to_string(dannoEnemigo) + " danno."
                + " (tuyo HP: " + to_string(juga.getPokemon(activoIdx)->getHpActual()) + "/" + to_string(juga.getPokemon(activoIdx)->getHPMAX()) + ")");

        if (juga.getPokemon(activoIdx)->estaMuerto()) {
            log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " fue debilitado.");
            // Switch to next alive pokemon
            activoIdx = -1;
            for (int i = 0; i < juga.getTamano(); i++) {
                if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
            }
            if (activoIdx == -1) break;
            log.log("  Adelante " + juga.getPokemon(activoIdx)->getNombre() + "!");
        }
    }

    bool victoria = pokemonSalvaje.estaMuerto();
    if (victoria) {
        log.log("  Victoria! " + juga.getPokemon(activoIdx)->getNombre() + " gano la batalla.");
        int exp = pokemonSalvaje.getNivel() * 50;
        juga.getPokemon(activoIdx)->ganarExperiencia(exp);
        log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " gano " + to_string(exp) + " EXP.");
    } else {
        log.log("  Derrota en batalla salvaje.");
    }
    return victoria;
}

// Fully automatic trainer battle with proper turn loop
bool Pelea::empezarBatallaEntrenador(Jugador& juga, Entrenador entrenador) {
    Logger& log = Logger::getInstancia();
    log.log("[Batalla] " + juga.getNombre() + " vs " + entrenador.getNombre());

    int enemigoIdx = 0;
    while (enemigoIdx < (int)entrenador.getEquipo().size()) {

        // Find active player pokemon
        int activoIdx = -1;
        for (int i = 0; i < juga.getTamano(); i++) {
            if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
        }
        if (activoIdx == -1) break;

        // Advance to next alive enemy
        while (enemigoIdx < (int)entrenador.getEquipo().size() &&
               entrenador.getPokemon(enemigoIdx).estaMuerto()) {
            enemigoIdx++;
        }
        if (enemigoIdx >= (int)entrenador.getEquipo().size()) break;

        Pokemon& enemigo = entrenador.getPokemon(enemigoIdx);
        log.log("  " + juga.getPokemon(activoIdx)->getNombre()
                + " (Nv." + to_string(juga.getPokemon(activoIdx)->getNivel()) + ")"
                + " vs " + enemigo.getNombre()
                + " (Nv." + to_string(enemigo.getNivel()) + ")");

        // Full turn loop for this matchup
        int turno = 0;
        while (!enemigo.estaMuerto() && !juga.getPokemon(activoIdx)->estaMuerto()) {
            turno++;

            // Player attacks first
            int dannoJugador = calcularDannio(*juga.getPokemon(activoIdx), enemigo);
            enemigo.dannioRecibido(dannoJugador);
            log.log("    Turno " + to_string(turno) + ": "
                    + juga.getPokemon(activoIdx)->getNombre()
                    + " ataca por " + to_string(dannoJugador) + "."
                    + " Rival HP: " + to_string(enemigo.getHpActual()) + "/" + to_string(enemigo.getHPMAX()));

            if (enemigo.estaMuerto()) break;

            // Enemy attacks back
            int dannoEnemigo = calcularDannio(enemigo, *juga.getPokemon(activoIdx));
            juga.getPokemon(activoIdx)->dannioRecibido(dannoEnemigo);
            log.log("    Turno " + to_string(turno) + ": "
                    + enemigo.getNombre()
                    + " contraataca por " + to_string(dannoEnemigo) + "."
                    + " Tu HP: " + to_string(juga.getPokemon(activoIdx)->getHpActual()) + "/" + to_string(juga.getPokemon(activoIdx)->getHPMAX()));
        }

        if (enemigo.estaMuerto()) {
            int exp = enemigo.getNivel() * 50;
            juga.getPokemon(activoIdx)->ganarExperiencia(exp);
            log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " gano " + to_string(exp) + " exp.");
            enemigoIdx++;
        } else {
            // Player's pokemon fainted — switch to next
            log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " fue debilitado.");
            activoIdx = -1;
            for (int i = 0; i < juga.getTamano(); i++) {
                if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
            }
            if (activoIdx == -1) break;
            log.log("  Adelante " + juga.getPokemon(activoIdx)->getNombre() + "!");
        }
    }

    bool victoria = juga.tienePokemonVivo();
    if (victoria) {
        juga.agregarPlata(entrenador.getPlataGanada());
        log.log("  Victoria! Ganaste " + to_string(entrenador.getPlataGanada()) + " monedas.");
    } else {
        log.log("  Derrota contra " + entrenador.getNombre() + ".");
    }
    return victoria;
}

int Pelea::seleccionarPokemon(Jugador& jugador) {
    for (int i = 0; i < jugador.getTamano(); i++) {
        Pokemon* p = jugador.getPokemon(i);
        if (!p->estaMuerto()) return i;
    }
    return -1;
}