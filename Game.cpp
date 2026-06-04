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
#include "EventoAtrapar.h"
#include "EventoNPC.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Game::Game() : ubicacionActual(0), muerto(false) {}

void Game::elegirStarter() {
    Logger& log = Logger::getInstancia();
    const auto& especies = loader.getEspecie();
    if (especies.empty()) {
        log.log("Error: No hay especies cargadas para elegir starter.");
        return;
    }

    int opciones = (int)especies.size() < 3 ? (int)especies.size() : 3;
    cout << "\n=== Elige tu Pokémon inicial ===" << endl;
    for (int i = 0; i < opciones; i++) {
        cout << "  [" << (i + 1) << "] " << especies[i].getNombre() << endl;
    }

    int eleccion = 0;
    while (eleccion < 1 || eleccion > opciones) {
        cout << "Tu elección (1-" << opciones << "): ";
        cin >> eleccion;
        if (eleccion < 1 || eleccion > opciones)
            cout << "Opción inválida, intenta de nuevo." << endl;
    }

    Pokemon starter = PokemonFactory::crearPoke(especies[eleccion - 1], 5);
    jugador.agregarPokemon(starter);
    log.log("Elegiste a " + starter.getNombre() + " como tu Pokémon inicial!");
}

void Game::inicializar() {
    srand((unsigned)time(nullptr));
    Logger& log = Logger::getInstancia();
    log.log("Iniciando simulación Pokémon...");

    if (!loader.cargarEspecies("data/especies.txt")) {
        log.log("Advertencia: No se cargaron especies.");
    } else {
        log.log("Especies cargadas: " + std::to_string(loader.getEspecie().size()));
    }
    if (!loader.cargarItem("data/items.txt")) {
        log.log("Advertencia: No se cargaron ítems.");
    } else {
        log.log("Ítems cargados: " + std::to_string(loader.getItems().size()));
    }
    if (!loader.cargarLiderGym("data/lideres.txt")) {
        log.log("Advertencia: No se cargaron líderes de gimnasio.");
    } else {
        log.log("Líderes cargados: " + std::to_string(loader.getLiderGym().size()));
    }

    world = GeneradorMapa::generarMundo("data/mapa.txt");
    log.log("Lugares en el mundo: " + std::to_string(world.getLugares().size()));

    objetivos.emplace_back("Conseguir 3 medallas de gimnasio");
    objetivos.emplace_back("Sobrevivir la aventura completa");

    jugador = Jugador("Ash");
    elegirStarter();

    log.log("Datos cargados. Comenzando aventura...");
    log.log("");
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
                // nivel escala con la zona: id/2 + 3, con algo de aleatoriedad
                int nivelZona = 3 + (lugar->getId() / 2) + rand() % 3;
                Pokemon salvaje = PokemonFactory::crearPokeRandom(especies, nivelZona);
                evento = new EventoPokemonSalvaje(salvaje);
            }
            break;
        case TipoEvento::TRAINER: {
            Entrenador ent("Entrenador " + std::to_string(lugar->getId()), 100 + lugar->getId() * 10);
            if (!especies.empty()) {
                int nivelZona = 4 + (lugar->getId() / 2) + rand() % 3;
                Pokemon p1 = PokemonFactory::crearPokeRandom(especies, nivelZona);
                ent.addPokemon(p1);
                // entrenadores más avanzados tienen 2 pokemon
                if (lugar->getId() > 10) {
                    Pokemon p2 = PokemonFactory::crearPokeRandom(especies, nivelZona);
                    ent.addPokemon(p2);
                }
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
        case TipoEvento::TRAP:
            evento = new EventoAtrapar(10);
            break;
        case TipoEvento::NPC:
            evento = new EventoNPC("¡Hola entrenador! Sigue adelante, la Liga te espera.");
            break;
        case TipoEvento::GYM: {
            int idx = jugador.getMedallas();
            if (!lideres.empty()) {
                idx = idx % (int)lideres.size();
                evento = new EventoGimnasio(lideres[idx]);
            }
            break;
        }
        case TipoEvento::LEAGUE: {
            log.log("[Liga] ¡Has llegado a la Liga Pokémon! ¡Batalla final!");
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
        Logger::getInstancia().log("\n[!] Tu equipo ha caído. Fin de la aventura.");
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
        log.log("  (vacío)");
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
        log.log("El mundo no tiene lugares. Verifica data/mapa.txt");
        return;
    }

    Lugar* actual = lugares[0];

    while (!muerto && actual != nullptr) {
        procesarLugar(actual);
        revisarDerrota();
        if (muerto) break;

        const auto& conexiones = actual->getSiguienteLugar();
        if (conexiones.empty()) break;

        cout << "\n=== ¿A dónde quieres ir? ===" << endl;
        vector<Lugar*> opciones;
        for (int id : conexiones) {
            Lugar* sig = world.getLugar(id);
            if (sig) opciones.push_back(sig);
        }
        for (int i = 0; i < (int)opciones.size(); i++) {
            cout << "  [" << (i+1) << "] " << opciones[i]->getNombre() << endl;
        }

        int elec = 0;
        while (elec < 1 || elec > (int)opciones.size()) {
            cout << "Tu elección: ";
            cin >> elec;
        }
        actual = opciones[elec - 1];
    }

    revisarVictoria();
    generarReporteFinal();
    log.close();
}
