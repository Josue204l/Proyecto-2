//
// Created by Gabri on 5/31/26.
//

#include "Game.h"
#include "GeneradorMapa.h"
#include "Logger.h"
#include "PokemonFactory.h"
#include "EventoPokemonSalvaje.h"
#include "EventoBatallaEntrenador.h"
#include "EventoGimnasio.h"
#include "EventoCentroPokemon.h"
#include "EventoTiendaObjetos.h"
#include "EventoTesoro.h"
#include "EventoNPC.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : ubicacionActual(0), muerto(false) {}

void Game::elegirStarter() {
    Logger& log = Logger::getInstancia();
    const auto& especies = loader.getEspecie();
    if (especies.empty()) {
        log.log("[Error] No hay especies cargadas para elegir starter.");
        return;
    }
    // Pick first 3 species as starters (or fewer if not enough)
    int opciones = (int)especies.size() < 3 ? (int)especies.size() : 3;
    log.log("\n=== Elige tu Pokemon inicial ===");
    for (int i = 0; i < opciones; i++) {
        log.log("  [" + std::to_string(i) + "] " + especies[i].getNombre());
    }
    // Auto-select index 0 for simulation
    int eleccion = 0;
    Pokemon starter = PokemonFactory::crearPoke(especies[eleccion], 5);
    jugador.agregarPokemon(starter);
    log.log("Elegiste a " + starter.getNombre() + " como tu Pokemon inicial!");
}

void Game::inicializar() {
    srand((unsigned)time(nullptr));
    Logger& log = Logger::getInstancia();
    log.log("========================================");
    log.log("   MOTOR DE SIMULACION POKEMON");
    log.log("========================================");

    // Load data files
    if (!loader.cargarEspecies("data/especies.txt")) {
        log.log("[Advertencia] No se cargaron especies.");
    } else {
        log.log("[Init] Especies cargadas: " + std::to_string(loader.getEspecie().size()));
    }
    if (!loader.cargarItem("data/items.txt")) {
        log.log("[Advertencia] No se cargaron items.");
    } else {
        log.log("[Init] Items cargados: " + std::to_string(loader.getItems().size()));
    }
    if (!loader.cargarLiderGym("data/lideres.txt")) {
        log.log("[Advertencia] No se cargaron lideres de gimnasio.");
    } else {
        log.log("[Init] Lideres cargados: " + std::to_string(loader.getLiderGym().size()));
    }

    // Build world
    world = GeneradorMapa::generarMundo("data/mapa.txt");
    log.log("[Init] Lugares en el mundo: " + std::to_string(world.getLugares().size()));

    // Set up objectives
    objetivos.emplace_back("Conseguir 3 medallas de gimnasio");
    objetivos.emplace_back("Sobrevivir la aventura completa");

    // Set player name and starter
    jugador = Jugador("Ash");
    elegirStarter();

    log.log("[Init] Simulacion lista. Comenzando aventura...");
    log.log("========================================\n");
}

void Game::procesarLugar(Lugar* lugar) {
    Logger& log = Logger::getInstancia();
    log.log("\n--- Lugar: " + lugar->getNombre() + " (" + std::to_string(lugar->getId()) + ") ---");

    const auto& especies = loader.getEspecie();
    const auto& items    = loader.getItems();
    const auto& lideres  = loader.getLiderGym();

    Evento* evento = nullptr;

    switch (lugar->getTipoEvento()) {
        case TipoEvento::WILD_POKEMON:
            if (!especies.empty()) {
                int nivel = 3 + rand() % 5;
                Pokemon salvaje = PokemonFactory::crearPokeRandom(especies, nivel);
                evento = new EventoPokemonSalvaje(salvaje);
            }
            break;
        case TipoEvento::TRAINER: {
            // Build a random trainer with 1-2 random pokemon
            Entrenador ent("Entrenador " + std::to_string(lugar->getId()), 150);
            if (!especies.empty()) {
                Pokemon p1 = PokemonFactory::crearPokeRandom(especies, 4 + rand() % 4);
                ent.addPokemon(p1);
            }
            evento = new EventoBatallaEntrenador(ent);
            break;
        }
        case TipoEvento::SHOP:
            if (!items.empty()) {
                evento = new EventoTiendaObjetos(items);
            }
            break;
        case TipoEvento::POKEMON_CENTER:
            evento = new EventoCentroPokemon();
            break;
        case TipoEvento::TREASURE:
            if (!items.empty()) {
                int idx = rand() % (int)items.size();
                evento = new EventoTesoro(items[idx]);
            }
            break;
        case TipoEvento::TRAP: {
            // Trap deals direct damage to active pokemon
            log.log("[Trampa] Caiste en una trampa! Tu Pokemon activo recibe 10 de danno.");
            Pokemon* activo = jugador.getPokemonActiv();
            if (activo) activo->dannioRecibido(10);
            break;
        }
        case TipoEvento::NPC:
            evento = new EventoNPC("Hola entrenador! Sigue adelante, la Liga te espera.");
            break;
        case TipoEvento::GYM: {
            // Use gym leaders in order by medal count
            int idx = jugador.getMedallas();
            if (!lideres.empty()) {
                idx = idx % (int)lideres.size();
                evento = new EventoGimnasio(lideres[idx]);
            }
            break;
        }
        case TipoEvento::LEAGUE: {
            log.log("[Liga] Has llegado a la Liga Pokemon! Batalla final!");
            if (!lideres.empty()) {
                evento = new EventoGimnasio(lideres.back());
            }
            break;
        }
    }

    if (evento) {
        evento->ejecutar(jugador);
        delete evento;
    }
}

void Game::revisarVictoria() {
    if (jugador.getMedallas() >= 3) {
        objetivos[0].completar();
    }
    if (!muerto) {
        objetivos[1].completar();
    }
}

void Game::revisarDerrota() {
    if (!jugador.tienePokemonVivo()) {
        muerto = true;
        Logger::getInstancia().log("\n[!] Tu equipo ha caido. Fin de la aventura.");
    }
}

void Game::generarReporteFinal() {
    Logger& log = Logger::getInstancia();
    log.log("\n========================================");
    log.log("         REPORTE FINAL");
    log.log("========================================");
    log.log("Jugador: " + jugador.getNombre());
    log.log("Medallas obtenidas: " + std::to_string(jugador.getMedallas()));
    log.log("Monedas finales: " + std::to_string(jugador.getPlata()));
    log.log("Estado: " + std::string(muerto ? "DERROTA" : "VICTORIA"));
    log.log("\n--- Equipo final ---");
    for (const auto& p : jugador.getEquipo()) {
        log.log("  " + p.getNombre() +
                " | Nivel: " + std::to_string(p.getNivel()) +
                " | HP: " + std::to_string(p.getHpActual()) + "/" + std::to_string(p.getHPMAX()) +
                (p.estaMuerto() ? " [DEBILITADO]" : " [VIVO]"));
    }
    log.log("\n--- Inventario final ---");
    if (jugador.getInventario().estaVacio()) {
        log.log("  (vacio)");
    } else {
        for (const auto& item : jugador.getInventario().getItems()) {
            log.log("  " + const_cast<Item&>(item).getNombre());
        }
    }
    log.log("\n--- Objetivos ---");
    for (const auto& obj : objetivos) {
        log.log("  [" + std::string(obj.estaCompletado() ? "X" : " ") + "] " + obj.getDescripcion());
    }
    log.log("========================================");

    // Write separate report file
    std::ofstream reporte("data/reporte_final.txt");
    if (reporte.is_open()) {
        reporte << "REPORTE FINAL - Motor de Simulacion Pokemon\n";
        reporte << "Jugador: " << jugador.getNombre() << "\n";
        reporte << "Medallas: " << jugador.getMedallas() << "\n";
        reporte << "Monedas: " << jugador.getPlata() << "\n";
        reporte << "Resultado: " << (muerto ? "DERROTA" : "VICTORIA") << "\n";
        reporte << "\nEquipo:\n";
        for (const auto& p : jugador.getEquipo()) {
            reporte << "  " << p.getNombre()
                    << " Nivel " << p.getNivel()
                    << " HP " << p.getHpActual() << "/" << p.getHPMAX()
                    << (p.estaMuerto() ? " DEBILITADO" : " VIVO") << "\n";
        }
        reporte << "\nObjetivos:\n";
        for (const auto& obj : objetivos) {
            reporte << "  [" << (obj.estaCompletado() ? "X" : " ") << "] " << obj.getDescripcion() << "\n";
        }
        reporte.close();
    }
}

void Game::correr() {
    Logger& log = Logger::getInstancia();
    const auto& lugares = world.getLugares();
    if (lugares.empty()) {
        log.log("[Error] El mundo no tiene lugares. Verifica data/mapa.txt");
        return;
    }

    // Traverse all locations in order
    for (int i = 0; i < (int)lugares.size() && !muerto; i++) {
        procesarLugar(lugares[i]);
        revisarDerrota();
    }

    revisarVictoria();
    generarReporteFinal();
    log.close();
}