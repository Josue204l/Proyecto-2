//
// Created by Gabri on 5/31/26.
//

#include "PokemonCenterEvent.h"
#include "Logger.h"

PokemonCenterEvent::PokemonCenterEvent() {}

void PokemonCenterEvent::ejecutar(Player& jug) {
    Logger::getInstancia().log("[Centro Pokémon] ¡Bienvenido! Curamos a tu equipo.");
    jug.curarEquipo();
    Logger::getInstancia().log("[Centro Pokémon] Tu equipo está completamente curado.");
}
