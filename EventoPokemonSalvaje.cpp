#include "EventoPokemonSalvaje.h"
#include "Pelea.h"
#include "Logger.h"

EventoPokemonSalvaje::EventoPokemonSalvaje(const Pokemon& p) : p(p) {}

// Automatic capture attempt based on remaining HP
bool EventoPokemonSalvaje::intentarCaptura(Jugador& jugador, Pokemon& salvaje) {
    if (jugador.equipoLleno()) {
        Logger::getInstancia().log("  Equipo lleno, no puedes capturar.");
        return false;
    }
    double vidaRestante = (double)salvaje.getHpActual() / salvaje.getHPMAX();
    int probabilidad = 80 - (int)(vidaRestante * 60);
    int roll = rand() % 100;
    if (roll < probabilidad) {
        jugador.agregarPokemon(salvaje);
        Logger::getInstancia().log("  " + salvaje.getNombre() + " fue capturado!");
        return true;
    }
    Logger::getInstancia().log("  La captura fallo.");
    return false;
}

// Fully automatic: battle first, then attempt capture if won
void EventoPokemonSalvaje::ejecutar(Jugador& jugador) {
    Logger& log = Logger::getInstancia();
    log.log("[Evento] Aparece un Pokemon salvaje: " + p.getNombre()
            + " (Nivel " + std::to_string(p.getNivel()) + ")");

    // Auto-battle
    bool gano = Pelea::empezarBatallaSalvaje(jugador, p);

    // After winning, try to capture if team isn't full
    if (gano && !jugador.equipoLleno()) {
        intentarCaptura(jugador, p);
    }
}