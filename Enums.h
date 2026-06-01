#ifndef PROYECTO_2_ENUMS_H
#define PROYECTO_2_ENUMS_H

// Tipos de Pokemon para calcular ventajas en combate
enum class TipoPokemon {
    NORMAL,
    FIRE,
    WATER,
    GRASS,
    ELECTRIC,
    ROCK,
    FLYING
};

// Tipos de evento que puede tener cada lugar del mapa
enum class TipoEvento {
    WILD_POKEMON,    // aparece un pokemon salvaje
    TRAINER,         // batalla contra un entrenador
    SHOP,            // tienda de objetos
    POKEMON_CENTER,  // cura a todos los pokemon
    TREASURE,        // encontras un item gratis
    TRAP,            // trampa que hace danno
    NPC,             // personaje que da un mensaje
    GYM,             // batalla contra lider de gimnasio
    LEAGUE           // batalla final de la liga
};

// Tipos de item que puede tener el jugador
enum class TipoItem {
    POKEBALL,
    GREATBALL,
    POTION,
    SUPER_POTION,
    RARE_CANDY
};

// Rareza de cada especie de pokemon
enum class Rareza {
    COMMON,
    RARE,
    EPIC,
    LEGENDARY
};

#endif // PROYECTO_2_ENUMS_H
