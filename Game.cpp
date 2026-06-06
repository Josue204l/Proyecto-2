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
        log.log("Error: No hay especies cargadas.");
        return;
    }

    // Agarro solo las primeras 3 porque al inicio demasiadas opciones mas bien estorban.
    int opciones = (int)especies.size() < 3 ? (int)especies.size() : 3;
    cout << "\n=== Elige tu Pokemon inicial ===" << endl;
    for (int i = 0; i < opciones; i++) {
        cout << "  [" << (i + 1) << "] " << especies[i].getNombre() << endl;
    }

    int eleccion = 0;
    while (eleccion < 1 || eleccion > opciones) {
        cout << "Tu eleccion (1-" << opciones << "): ";
        cin >> eleccion;
        if (eleccion < 1 || eleccion > opciones)
            cout << "Opcion invalida." << endl;
    }

    Pokemon starter = PokemonFactory::crearPoke(especies[eleccion - 1], 5);
    jugador.agregarPokemon(starter);
    log.log("Elegiste a " + starter.getNombre() + " como tu Pokemon inicial!");

    // Le doy unas cosillas de arranque para que el jugador no quede vendido apenas empieza.
    const auto& items = loader.getItems();
    for (const auto& item : items) {
        if (const_cast<Item&>(item).getTipo() == TipoItem::POKEBALL) {
            jugador.getInventario().agregarItem(const_cast<Item&>(item));
            jugador.getInventario().agregarItem(const_cast<Item&>(item));
            jugador.getInventario().agregarItem(const_cast<Item&>(item));
            log.log("Recibiste 3 Pokeballs!");
            break;
        }
    }
    for (const auto& item : items) {
        if (const_cast<Item&>(item).getTipo() == TipoItem::POTION) {
            jugador.getInventario().agregarItem(const_cast<Item&>(item));
            jugador.getInventario().agregarItem(const_cast<Item&>(item));
            log.log("Recibiste 2 Pociones!");
            break;
        }
    }
}

void Game::inicializar() {
    srand((unsigned)time(nullptr));
    Logger& log = Logger::getInstancia();
    log.log("Iniciando simulacion Pokemon...");

    if (!loader.cargarEspecies("data/especies.txt"))
        log.log("Advertencia: No se cargaron especies.");
    else
        log.log("Especies cargadas: " + to_string(loader.getEspecie().size()));

    if (!loader.cargarItem("data/items.txt"))
        log.log("Advertencia: No se cargaron items.");
    else
        log.log("Items cargados: " + to_string(loader.getItems().size()));

    if (!loader.cargarLiderGym("data/lideres.txt"))
        log.log("Advertencia: No se cargaron lideres.");
    else
        log.log("Lideres cargados: " + to_string(loader.getLiderGym().size()));

    world = GeneradorMapa::generarMundo("data/mapa.txt");
    log.log("Lugares en el mundo: " + to_string(world.getLugares().size()));

    objetivos.emplace_back("Conseguir 3 medallas de gimnasio");
    objetivos.emplace_back("Derrotar a Giovanni en la Liga");

    jugador = Jugador("Ash");
    elegirStarter();

    log.log("Aventura comenzando...\n");
}

void Game::procesarLugar(Lugar* lugar) {
    Logger& log = Logger::getInstancia();
    log.log("\n--- " + lugar->getNombre() + " ---");

    const auto& especies = loader.getEspecie();
    const auto& items    = loader.getItems();
    const auto& lideres  = loader.getLiderGym();

    Evento* evento = nullptr;

    switch (lugar->getTipoEvento()) {
        case TipoEvento::WILD_POKEMON:
            if (!especies.empty()) {
                // Esto escala con las medallas para que el arranque no se vuelva injusto desde la primera vuelta.
                std::vector<EspeciePokemon> pool;
                for (const auto& esp : especies) {
                    if (esp.getRareza() == Rareza::LEGENDARY) continue;
                    if (jugador.getMedallas() == 0 && esp.getRareza() != Rareza::COMMON) continue;
                    if (jugador.getMedallas() == 1 && esp.getRareza() == Rareza::EPIC) continue;
                    pool.push_back(esp);
                }
                if (pool.empty()) pool.push_back(especies[0]);
                int idx = rand() % (int)pool.size();
                int nivel = 3 + rand() % (jugador.getMedallas() * 3 + 4);
                Pokemon salvaje = PokemonFactory::crearPoke(pool[idx], nivel);
                evento = new EventoPokemonSalvaje(salvaje);
            }
            break;

        case TipoEvento::TRAINER: {
            Entrenador ent("Entrenador " + lugar->getNombre(), 150 + jugador.getMedallas() * 50);
            if (!especies.empty()) {
                int nivel = 4 + jugador.getMedallas() * 3 + rand() % 4;
                // Los primeros entrenadores salen flojitos apropósito, la idea es que enseñen el ritmo del juego y no frenen la partida.
                // Ya después si se abre el pool completo para que la progresión se sienta real.
                int maxPool = (int)especies.size() - 1; // saco legendarios porque aparecer tan pronto rompería el balance feisimo
                if (jugador.getMedallas() == 0) maxPool = min(maxPool, 10); // acá recorto el pool para que el inicio no pegue tan duro
                int idx = rand() % maxPool;
                Pokemon p1 = PokemonFactory::crearPoke(especies[idx], nivel);
                ent.addPokemon(p1);
                // Meto un segundo pokemon después porque así la dificultad sube poquito a poco y no de golpe.
                if (jugador.getMedallas() >= 1) {
                    int idx2 = rand() % maxPool;
                    Pokemon p2 = PokemonFactory::crearPoke(especies[idx2], nivel - 1);
                    ent.addPokemon(p2);
                }
            }
            evento = new EventoBatallaEntrenador(ent);
            break;
        }

        case TipoEvento::SHOP:
            if (!items.empty()) evento = new EventoTiendaObjetos(items);
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
            evento = new EventoNPC("Hola entrenador! Sigue adelante, la Liga te espera.");
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
            log.log("[Liga] Has llegado a la Liga Pokemon!");
            if (jugador.getMedallas() < 3) {
                log.log("[Liga] Necesitas 3 medallas para entrar. Solo tienes "
                        + to_string(jugador.getMedallas()) + ".");
                log.log("[Liga] Regresa cuando tengas mas medallas.");
                return;
            }
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
    if (jugador.getMedallas() >= 3) objetivos[0].completar();
    // La victoria no se marca solo por llegar; tiene que llegar vivo, si no el cierre se siente medio falso.
    if (!muerto && ubicacionActual == (int)world.getLugares().size() - 1) {
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
    log.log("Medallas: " + to_string(jugador.getMedallas()));
    log.log("Monedas: "  + to_string(jugador.getPlata()));
    log.log("Estado: "   + string(muerto ? "DERROTA" : "VICTORIA"));
    log.log("\n--- Equipo final ---");
    for (const auto& p : jugador.getEquipo()) {
        log.log("  " + p.getNombre()
                + " Nv." + to_string(p.getNivel())
                + " HP "  + to_string(p.getHpActual()) + "/" + to_string(p.getHPMAX())
                + (p.estaMuerto() ? " [DEBILITADO]" : " [VIVO]"));
    }
    log.log("\n--- Inventario ---");
    if (jugador.getInventario().estaVacio()) {
        log.log("  (vacio)");
    } else {
        for (const auto& item : jugador.getInventario().getItems()) {
            log.log("  " + const_cast<Item&>(item).getNombre());
        }
    }
    log.log("\n--- Objetivos ---");
    for (const auto& obj : objetivos) {
        log.log("  [" + string(obj.estaCompletado() ? "X" : " ") + "] " + obj.getDescripcion());
    }
    log.log("========================================");

    ofstream reporte("data/reporte_final.txt");
    if (reporte.is_open()) {
        reporte << "REPORTE FINAL - Motor de Simulacion Pokemon\n";
        reporte << "Jugador: "   << jugador.getNombre()  << "\n";
        reporte << "Medallas: "  << jugador.getMedallas() << "\n";
        reporte << "Monedas: "   << jugador.getPlata()    << "\n";
        reporte << "Resultado: " << (muerto ? "DERROTA" : "VICTORIA") << "\n";
        reporte << "\nEquipo:\n";
        for (const auto& p : jugador.getEquipo()) {
            reporte << "  " << p.getNombre()
                    << " Nv." << p.getNivel()
                    << " HP "  << p.getHpActual() << "/" << p.getHPMAX()
                    << (p.estaMuerto() ? " DEBILITADO" : " VIVO") << "\n";
        }
        reporte << "\nObjetivos:\n";
        for (const auto& obj : objetivos) {
            reporte << "  [" << (obj.estaCompletado() ? "X" : " ") << "] " << obj.getDescripcion() << "\n";
        }
        reporte.close();
        log.log("Reporte guardado en data/reporte_final.txt");
    }
}

void Game::correr() {
    Logger& log = Logger::getInstancia();
    Lugar* actual = world.getLugar(ubicacionActual);

    if (actual == nullptr) {
        log.log("Error: no existe el lugar inicial.");
        return;
    }

    while (!muerto) {
        procesarLugar(actual);
        revisarDerrota();
        if (muerto) break;

        const vector<int>& conexiones = actual->getConexiones();
        if (conexiones.empty()) break;

        // Muestro destinos acá para que el jugador vea el contexto antes de decidir y no ande escogiendo a ciegas.
        cout << "\nPuedes viajar a:\n";
        for (int i = 0; i < (int)conexiones.size(); i++) {
            Lugar* destino = world.getLugar(conexiones[i]);
            if (destino != nullptr)
                cout << "  " << (i + 1) << ". " << destino->getNombre() << "\n";
        }

        int opcion = 0;
        cout << "Destino: ";
        cin >> opcion;
        while (opcion < 1 || opcion > (int)conexiones.size()) {
            cout << "Opcion invalida: ";
            cin >> opcion;
        }

        ubicacionActual = conexiones[opcion - 1];
        actual = world.getLugar(ubicacionActual);
    }

    revisarVictoria();
    generarReporteFinal();
    log.close();
}