#include "Game.h"
#include "MapGenerator.h"
#include "Logger.h"
#include "PokemonFactory.h"
#include "WildPokemonEvent.h"
#include "TrainerBattleEvent.h"
#include "GymEvent.h"
#include "PokemonCenterEvent.h"
#include "ShopEvent.h"
#include "TreasureEvent.h"
#include "TrapEvent.h"
#include "NpcEvent.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;
static string tipoPokemonStr(TipoPokemon t) {
    switch (t) {
        case TipoPokemon::FIRE:     return "Fuego";
        case TipoPokemon::WATER:    return "Agua";
        case TipoPokemon::GRASS:    return "Planta";
        case TipoPokemon::ELECTRIC: return "Electrico";
        case TipoPokemon::ROCK:     return "Roca";
        case TipoPokemon::FLYING:   return "Volador";
        case TipoPokemon::NORMAL:   return "Normal";
        default:                    return "Desconocido";
    }
}

static int obtenerZona(int idLugar) {
    if (idLugar <= 6) return 0;
    if (idLugar <= 12) return 1;
    if (idLugar <= 18) return 2;
    return 3;
}

Game::Game()
    : ubicacionActual(0), muerto(false), inicializado(false), ligaGanada(false),
      evolucionesRealizadas(0) {}

void Game::elegirStarter() {
    Logger& log = Logger::getInstancia();
    const auto& especies = loader.getEspecie();

    // Filtrar solo los starters
    vector<PokemonSpecies> starters;
    for (const auto& esp : especies)
        if (esp.esStarter()) starters.push_back(esp);

    if (starters.empty()) {
        log.log("Error: No hay starters definidos en especies.txt");
        return;
    }

    cout << "\n=== Elige tu Pokemon inicial ===\n" << endl;
    for (int i = 0; i < (int)starters.size(); i++) {
        cout << "  [" << (i + 1) << "] " << starters[i].getNombre()
             << "  Tipo: " << tipoPokemonStr(starters[i].getTipo())
             << "  HP: "   << starters[i].getHpBase()
             << "  ATK: "  << starters[i].getAtaqueBase()
             << "  DEF: "  << starters[i].getDefenseBase() << "\n"
             << "      Evoluciona en: " << starters[i].getNombreEvolucion()
             << " (nivel " << starters[i].getNivelEvolucion() << ")\n" << endl;
    }

    int eleccion = 0;
    while (eleccion < 1 || eleccion > (int)starters.size()) {
        cout << "Tu eleccion (1-" << starters.size() << "): ";
        cin >> eleccion;
        if (eleccion < 1 || eleccion > (int)starters.size())
            cout << "Opcion invalida." << endl;
    }

    Pokemon starter = PokemonFactory::crearPoke(starters[eleccion - 1], 5);
    jugador.agregarPokemon(starter);
    log.log("Elegiste a " + starter.getNombre() + " como tu Pokemon inicial!");

    // Items de arranque (esto no cambia)
    const auto& items = loader.getItems();
    for (const auto& item : items) {
        if (item.getTipo() == TipoItem::POKEBALL) {
            jugador.getInventario().agregarItem(item);
            jugador.getInventario().agregarItem(item);
            jugador.getInventario().agregarItem(item);
            log.log("Recibiste 3 Pokeballs!");
            break;
        }
    }
    for (const auto& item : items) {
        if (item.getTipo() == TipoItem::POTION) {
            jugador.getInventario().agregarItem(item);
            jugador.getInventario().agregarItem(item);
            log.log("Recibiste 2 Pociones!");
            break;
        }
    }
}

void Game::inicializar() {
    srand((unsigned)time(nullptr));
    Logger& log = Logger::getInstancia();
    log.reiniciar();
    log.log("Iniciando simulacion Pokemon...");
    log.log("Este juego es un roguelike: cada aventura puede tener encuentros y resultados diferentes.");

    const bool especiesCargadas = loader.cargarEspecies("data/especies.txt");
    const bool itemsCargados = loader.cargarItem("data/items.txt");
    const bool lideresCargados = loader.cargarLiderGym("data/lideres.txt");

    if (!especiesCargadas || !itemsCargados || !lideresCargados) {
        log.log("Error: no fue posible cargar todos los archivos de entrada.");
        return;
    }

    log.log("Especies cargadas: " + to_string(loader.getEspecie().size()));
    log.log("Items cargados: " + to_string(loader.getItems().size()));
    log.log("Lideres cargados: " + to_string(loader.getLiderGym().size()));

    world = MapGenerator::generarMundo("data/mapa.txt");
    if (world.getLugares().empty()) {
        log.log("Error: no fue posible construir el mundo desde data/mapa.txt.");
        return;
    }
    log.log("Lugares en el mundo: " + to_string(world.getLugares().size()));

    objetivos.emplace_back("Conseguir 3 medallas de gimnasio");
    objetivos.emplace_back("Formar un equipo de 6 Pokemon");
    objetivos.emplace_back("Evolucionar 3 Pokemon");
    objetivos.emplace_back("Alcanzar nivel 20 con un Pokemon");
    objetivos.emplace_back("Derrotar a Giovanni en la Liga");

    string nombre;
    cout << "\n¿Cómo te llamas, entrenador? ";
    cin >> nombre;
    jugador = Player(nombre);
    elegirStarter();
    if (jugador.getTamano() == 0) {
        log.log("Error: no fue posible asignar un Pokemon inicial.");
        return;
    }

    log.log("Aventura comenzando...\n");
    inicializado = true;
}

void Game::procesarLugar(Location* lugar) {
    Logger& log = Logger::getInstancia();
    log.log("\n--- " + lugar->getNombre() + " ---");

    const auto& especies = loader.getEspecie();
    const auto& items    = loader.getItems();
    const auto& lideres  = loader.getLiderGym();

    Event* evento = nullptr;

    switch (lugar->getTipoEvento()) {
        case TipoEvento::WILD_POKEMON:
            if (!especies.empty()) {
                int medallas = jugador.getMedallas();
                int zona = obtenerZona(lugar->getId());
                std::vector<PokemonSpecies> pool;

                for (const auto& esp : especies) {
                    // Nunca aparecen legendarios en la hierba
                    if (esp.getRareza() == Rareza::LEGENDARY) continue;

                    // Nunca aparecen formas finales (las que no evolucionan más y son EPIC)
                    // Eso filtra Venusaur, Charizard, Blastoise, etc.
                    if (esp.getRareza() == Rareza::EPIC) continue;

                    // Con 0 medallas: solo formas base (COMMON)
                    if (medallas == 0 && esp.getRareza() != Rareza::COMMON) continue;

                    // Con 1 medalla: base + primera evolución (COMMON y RARE)
                    // Con 2+: todo lo que no sea EPIC ni LEGENDARY (ya filtrado arriba)

                    pool.push_back(esp);
                }

                if (pool.empty()) pool.push_back(especies[0]);

                int idx = rand() % (int)pool.size();

                const int nivelMinimo[] = {3, 8, 13, 20};
                const int nivelMaximo[] = {6, 11, 17, 25};
                int nivelMin = nivelMinimo[zona];
                int nivelMax = nivelMaximo[zona];
                int nivel = nivelMin + rand() % (nivelMax - nivelMin + 1);

                Pokemon salvaje = PokemonFactory::crearPoke(pool[idx], nivel);
                evento = new WildPokemonEvent(salvaje);
            }
            break;
        case TipoEvento::TRAINER: {
            Trainer ent("Entrenador " + lugar->getNombre(), 150 + jugador.getMedallas() * 50);
            if (!especies.empty()) {
                int zona = obtenerZona(lugar->getId());
                const int nivelMinimo[] = {5, 9, 15, 22};
                const int nivelMaximo[] = {7, 12, 18, 27};
                int nivel = nivelMinimo[zona] +
                            rand() % (nivelMaximo[zona] - nivelMinimo[zona] + 1);
                // Los primeros entrenadores salen flojitos apropósito, la idea es que enseñen el ritmo del juego y no frenen la partida.
                // Ya después si se abre el pool completo para que la progresión se sienta real.
                // Mismo criterio que salvajes: sin épicos ni legendarios en el pool de entrenadores tempranos
                std::vector<PokemonSpecies> poolEnt;
                for (const auto& esp : especies) {
                    if (esp.getRareza() == Rareza::LEGENDARY) continue;
                    if (esp.getRareza() == Rareza::EPIC) continue;
                    if (jugador.getMedallas() == 0 && esp.getRareza() != Rareza::COMMON) continue;
                    poolEnt.push_back(esp);
                }
                if (poolEnt.empty()) poolEnt.push_back(especies[0]);
                int idxEnt = rand() % (int)poolEnt.size();
                Pokemon p1 = PokemonFactory::crearPoke(poolEnt[idxEnt], nivel);
                ent.addPokemon(p1);
                // Meto un segundo pokemon después porque así la dificultad sube poquito a poco y no de golpe.
                if (jugador.getMedallas() >= 1) {
                    int idx2 = rand() % (int)poolEnt.size();
                    int nivelSegundo = std::max(nivelMinimo[zona], nivel - 1);
                    Pokemon p2 = PokemonFactory::crearPoke(poolEnt[idx2], nivelSegundo);
                    ent.addPokemon(p2);
                }
            }
            evento = new TrainerBattleEvent(ent);
            break;
        }

        case TipoEvento::SHOP:
            if (!items.empty()) evento = new ShopEvent(items);
            break;

        case TipoEvento::POKEMON_CENTER:
            evento = new PokemonCenterEvent();
            break;

        case TipoEvento::TREASURE:
            if (!items.empty()) {
                int idx = rand() % (int)items.size();
                evento = new TreasureEvent(items[idx]);
            }
            break;

        case TipoEvento::TRAP:
            evento = new TrapEvent(10);
            break;

        case TipoEvento::NPC:
            evento = new NpcEvent("Hola entrenador! Sigue adelante, la Liga te espera.");
            break;

        case TipoEvento::GYM: {
            int idx = jugador.getMedallas();
            if (!lideres.empty()) {
                idx = idx % (int)lideres.size();
                evento = new GymEvent(lideres[idx]);
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
                evento = new GymEvent(lideres.back());
            }
            break;
        }
    }

    if (evento) {
        const int medallasAntes = jugador.getMedallas();
        evento->ejecutar(jugador);
        if (lugar->getTipoEvento() == TipoEvento::LEAGUE &&
            jugador.getMedallas() > medallasAntes) {
            ligaGanada = true;
        }
        delete evento;
    }
}

void Game::revisarVictoria() {
    if (jugador.getMedallas() >= 3) objetivos[0].completar();
    if (jugador.getTamano() >= 6) objetivos[1].completar();
    if (evolucionesRealizadas >= 3) objetivos[2].completar();
    for (const auto& pokemon : jugador.getEquipo()) {
        if (pokemon.getNivel() >= 20) {
            objetivos[3].completar();
            break;
        }
    }
    if (!muerto && ligaGanada) objetivos[4].completar();
}

void Game::revisarDerrota() {
    if (!jugador.tienePokemonVivo()) {
        muerto = true;
        Logger::getInstancia().log("\n[!] Tu equipo ha caido. Fin de la aventura.");
    }
}

void Game::generarReporteFinal() {
    Logger& log = Logger::getInstancia();
    const bool victoria = !muerto && ligaGanada && jugador.getMedallas() >= 3;
    const string resultado = muerto ? "DERROTA" : (victoria ? "VICTORIA" : "INCOMPLETA");
    log.log("\n========================================");
    log.log("         REPORTE FINAL");
    log.log("========================================");
    log.log("Jugador: " + jugador.getNombre());
    log.log("Medallas: " + to_string(jugador.getMedallas()));
    log.log("Monedas: "  + to_string(jugador.getPlata()));
    log.log("Evoluciones: " + to_string(evolucionesRealizadas));
    log.log("Estado: " + resultado);
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
            log.log("  " + item.getNombre());
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
        reporte << "Evoluciones: " << evolucionesRealizadas << "\n";
        reporte << "Resultado: " << resultado << "\n";
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
void Game::revisarEvoluciones() {
    const auto& especies = loader.getEspecie();
    for (int i = 0; i < jugador.getTamano(); i++) {
        Pokemon* p = jugador.getPokemon(i);
        if (p->puedeEvolucionar()) {
            string nombreEvo = p->getEspeciePokemon().getNombreEvolucion();
            for (const auto& esp : especies) {
                if (esp.getNombre() == nombreEvo) {
                    string nombreAntes = p->getNombre();
                    p->evolucionar(esp);
                    evolucionesRealizadas++;
                    Logger::getInstancia().log("  ¡" + nombreAntes + " evolucionó a " + esp.getNombre() + "!");
                    cout << "\n  *** ¡" + nombreAntes + " evolucionó a " + esp.getNombre() + "! ***\n" << endl;
                    break;
                }
            }
        }
    }
}

void Game::correr() {
    Logger& log = Logger::getInstancia();
    if (!inicializado) {
        log.log("La simulacion no puede iniciar porque la carga inicial fallo.");
        return;
    }
    Location* actual = world.getLugar(ubicacionActual);

    if (actual == nullptr) {
        log.log("Error: no existe el lugar inicial.");
        return;
    }

    while (!muerto) {
        procesarLugar(actual);
        revisarEvoluciones();
        revisarVictoria();
        revisarDerrota();
        if (muerto) break;

        const vector<int>& conexiones = actual->getConexiones();
        if (conexiones.empty()) break;

        // Muestro destinos acá para que el jugador vea el contexto antes de decidir y no ande escogiendo a ciegas.
        cout << "\nPuedes viajar a:\n";
        for (int i = 0; i < (int)conexiones.size(); i++) {
            Location* destino = world.getLugar(conexiones[i]);
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
}
