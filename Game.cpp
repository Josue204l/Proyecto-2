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
Game::Game() : ubicacionActual(0), muerto(false) {}

void Game::elegirStarter() {
    Logger& log = Logger::getInstancia();
    const auto& especies = loader.getEspecie();

    // Filtrar solo los starters
    vector<EspeciePokemon> starters;
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

    world = GeneradorMapa::generarMundoAleatorio(10);
    log.log("Lugares en el mundo: " + to_string(world.getLugares().size()));

    objetivos.emplace_back("Conseguir 3 medallas de gimnasio");
    objetivos.emplace_back("Derrotar a Giovanni en la Liga");

    string nombre;
    cout << "\n¿Cómo te llamas, entrenador? ";
    cin >> nombre;
    jugador = Jugador(nombre);
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
                int medallas = jugador.getMedallas();
                std::vector<EspeciePokemon> pool;

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

                // Nivel acotado: el máximo nunca supera el nivel del starter + margen justo
                // Con 0 medallas: nivel 2-5. Con 1: 5-9. Con 2: 9-13.
                int nivelMin = 2 + medallas * 4;
                int nivelMax = nivelMin + 3;
                int nivel = nivelMin + rand() % (nivelMax - nivelMin + 1);

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
                // Mismo criterio que salvajes: sin épicos ni legendarios en el pool de entrenadores tempranos
                std::vector<EspeciePokemon> poolEnt;
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
                    Pokemon p2 = PokemonFactory::crearPoke(poolEnt[idx2], nivel - 1);
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
    Lugar* actual = world.getLugar(ubicacionActual);

    if (actual == nullptr) {
        log.log("Error: no existe el lugar inicial.");
        return;
    }

    while (!muerto) {
        procesarLugar(actual);
        revisarEvoluciones();
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