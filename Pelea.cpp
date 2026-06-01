//
// Created by Gabri on 5/31/26.
//

#include "Pelea.h"
#include "Logger.h"
#include <string>

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

bool Pelea::empezarBatallaSalvaje(Jugador& juga, Pokemon pokemonSalvaje) {
    Logger& log = Logger::getInstancia();

    // Busca el indice del primer pokemon vivo
    int activoIdx = -1;
    for (int i = 0; i < juga.getTamano(); i++) {
        if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
    }
    if (activoIdx == -1) return false;

    log.log("  [Batalla] " + juga.getPokemon(activoIdx)->getNombre() + " vs " + pokemonSalvaje.getNombre() + " (salvaje)");

    while (true) {
        // Turno del jugador: ataca
        int dannoJugador = calcularDannio(*juga.getPokemon(activoIdx), pokemonSalvaje);
        pokemonSalvaje.dannioRecibido(dannoJugador);
        log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " ataca por " + std::to_string(dannoJugador) + " de danno.");

        if (pokemonSalvaje.estaMuerto()) break;

        // Turno del enemigo: contraataca
        int dannoEnemigo = calcularDannio(pokemonSalvaje, *juga.getPokemon(activoIdx));
        juga.getPokemon(activoIdx)->dannioRecibido(dannoEnemigo);
        log.log("  " + pokemonSalvaje.getNombre() + " contraataca por " + std::to_string(dannoEnemigo) + " de danno.");

        // Si el pokemon activo cayo, busca el siguiente vivo
        if (juga.getPokemon(activoIdx)->estaMuerto()) {
            activoIdx = -1;
            for (int i = 0; i < juga.getTamano(); i++) {
                if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
            }
            if (activoIdx == -1) break;
            log.log("  " + juga.getNombre() + " envia a " + juga.getPokemon(activoIdx)->getNombre() + "!");
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
    log.log("  [Batalla vs Entrenador] " + juga.getNombre() + " vs " + entrenador.getNombre());

    int enemigoIdx = 0;

    while (enemigoIdx < (int)entrenador.getEquipo().size()) {
        // Busca el pokemon activo del jugador por indice
        int activoIdx = -1;
        for (int i = 0; i < juga.getTamano(); i++) {
            if (!juga.getPokemon(i)->estaMuerto()) { activoIdx = i; break; }
        }
        if (activoIdx == -1) break; // jugador sin pokemon vivos

        // Avanza al siguiente enemigo vivo
        while (enemigoIdx < (int)entrenador.getEquipo().size() &&
               entrenador.getPokemon(enemigoIdx).estaMuerto()) {
            enemigoIdx++;
        }
        if (enemigoIdx >= (int)entrenador.getEquipo().size()) break;

        Pokemon& enemigo = entrenador.getPokemon(enemigoIdx);
        log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " vs " + enemigo.getNombre());

        // Turno del jugador: ataca
        int dannoJugador = calcularDannio(*juga.getPokemon(activoIdx), enemigo);
        enemigo.dannioRecibido(dannoJugador);
        log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " ataca por " + std::to_string(dannoJugador) + " de danno.");

        if (enemigo.estaMuerto()) {
            int exp = enemigo.getNivel() * 50;
            juga.getPokemon(activoIdx)->ganarExperiencia(exp);
            log.log("  " + juga.getPokemon(activoIdx)->getNombre() + " gano " + std::to_string(exp) + " exp.");
            enemigoIdx++;
            continue;
        }

        // Turno del enemigo: contraataca
        int dannoEnemigo = calcularDannio(enemigo, *juga.getPokemon(activoIdx));
        juga.getPokemon(activoIdx)->dannioRecibido(dannoEnemigo);
        log.log("  " + enemigo.getNombre() + " contraataca por " + std::to_string(dannoEnemigo) + " de danno.");
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