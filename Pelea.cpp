//
// Created by Gabri on 5/31/26.
//

#include "Pelea.h"
#include "Logger.h"
#include "AgresivaStrategy.h"
#include "RandomStrategy.h"
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
    Pokemon* activo = juga.getPokemonActiv();
    if (!activo) return false;

    log.log("  [Batalla] " + activo->getNombre() + " vs " + pokemonSalvaje.getNombre() + " (salvaje)");

    AgresivaStrategy estrategia;
    while (!activo->estaMuerto() && !pokemonSalvaje.estaMuerto()) {
        int accion = estrategia.elegirAccion();
        if (accion == 0) {
            int d = calcularDannio(*activo, pokemonSalvaje);
            pokemonSalvaje.dannioRecibido(d);
            log.log("  " + activo->getNombre() + " ataca por " + std::to_string(d) + " de danno.");
        }
        if (!pokemonSalvaje.estaMuerto()) {
            int d = calcularDannio(pokemonSalvaje, *activo);
            activo->dannioRecibido(d);
            log.log("  " + pokemonSalvaje.getNombre() + " contraataca por " + std::to_string(d) + " de danno.");
        }
        // Switch to next alive pokemon if current fainted
        if (activo->estaMuerto()) {
            activo = juga.getPokemonActiv();
            if (!activo) break;
            log.log("  " + juga.getNombre() + " envia a " + activo->getNombre() + "!");
        }
    }

    bool victoria = !pokemonSalvaje.estaMuerto() == false;
    if (victoria) {
        log.log("  Resultado: Victoria! " + pokemonSalvaje.getNombre() + " fue derrotado.");
        activo = juga.getPokemonActiv();
        if (activo) {
            int exp = pokemonSalvaje.getNivel() * 50;
            activo->ganarExperiencia(exp);
            log.log("  " + activo->getNombre() + " gano " + std::to_string(exp) + " de experiencia.");
            if (activo->puedeEvolucionar()) {
                log.log("  " + activo->getNombre() + " puede evolucionar!");
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

    RandomStrategy estrategia;
    int enemigoIdx = 0;

    while (juga.tienePokemonVivo() && entrenador.tienePokemonVivos()) {
        Pokemon* activo = juga.getPokemonActiv();
        if (!activo) break;

        // Find first alive enemy pokemon
        while (enemigoIdx < (int)entrenador.getEquipo().size() &&
               entrenador.getPokemon(enemigoIdx).estaMuerto()) {
            enemigoIdx++;
        }
        if (enemigoIdx >= (int)entrenador.getEquipo().size()) break;

        Pokemon& enemigo = entrenador.getPokemon(enemigoIdx);
        log.log("  " + activo->getNombre() + " vs " + enemigo.getNombre());

        int accion = estrategia.elegirAccion();
        if (accion == 0) {
            int d = calcularDannio(*activo, enemigo);
            enemigo.dannioRecibido(d);
            log.log("  " + activo->getNombre() + " ataca por " + std::to_string(d) + " de danno.");
        }
        if (!enemigo.estaMuerto()) {
            int d = calcularDannio(enemigo, *activo);
            activo->dannioRecibido(d);
            log.log("  " + enemigo.getNombre() + " contraataca por " + std::to_string(d) + " de danno.");
        } else {
            int exp = enemigo.getNivel() * 50;
            activo->ganarExperiencia(exp);
            log.log("  " + activo->getNombre() + " gano " + std::to_string(exp) + " exp.");
            enemigoIdx++;
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