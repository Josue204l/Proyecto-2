#include "WildPokemonEvent.h"
#include "Battle.h"
#include "Logger.h"

WildPokemonEvent::WildPokemonEvent(const Pokemon& p) : p(p) {}


bool WildPokemonEvent::intentarCaptura(Player& jugador, Pokemon& salvaje) {
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
void WildPokemonEvent::ejecutar(Player& jugador) {
    Logger& log = Logger::getInstancia();
    log.log("[Evento] Aparece un Pokemon salvaje: " + p.getNombre()
            + " (Nivel " + std::to_string(p.getNivel()) + ")");

    bool gano = Battle::empezarBatallaSalvaje(jugador, p);

    if (gano && !jugador.equipoLleno()) {
        intentarCaptura(jugador, p);
    }
}