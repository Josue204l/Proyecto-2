# Motor de Simulación Pokémon
**Curso:** EIF204 - Programación II  
**Carrera:** Ingeniería de Sistemas de Información  
**Universidad Nacional de Costa Rica**  
**Periodo:** I Ciclo 2026

---

## Descripción de la simulación

El sistema simula una aventura Pokémon completa en consola. El jugador recorre un mundo compuesto por 26 lugares interconectados, enfrentando Pokémon salvajes, entrenadores, líderes de gimnasio y eventos especiales. La simulación es semiautomática: las reglas controlan el mundo y el jugador elige rutas y acciones de combate.

El objetivo es obtener **3 medallas de gimnasio** y sobrevivir hasta llegar a la **Liga Pokémon**.

### Progresión de niveles

| Tramo | Pokémon salvajes | Entrenadores |
|---|---:|---:|
| Antes de Brock | 3-6 | 5-7 |
| Antes de Misty | 8-11 | 9-12 |
| Antes de Lt. Surge | 13-17 | 15-18 |
| Camino a la Liga | 20-25 | 22-27 |

---

## Estructura del proyecto

```
Proyecto#2/
├── main.cpp                  # Punto de entrada
├── Game.h / Game.cpp         # Motor principal de simulación
├── World.h / World.cpp       # Contenedor del mundo
├── Location.h / Location.cpp # Espacio navegable (hereda Space)
├── Space.h                   # Interfaz abstracta de espacio
├── Character.h               # Interfaz abstracta de personaje
├── Player.h / Player.cpp   # Jugador principal (hereda Character)
├── Trainer.h / Trainer.cpp
├── Pokemon.h / Pokemon.cpp   # Entidad Pokémon con stats y evolución
├── PokemonSpecies.h / .cpp   # Datos base de cada especie
├── Inventory.h / .cpp       # Inventario del jugador
├── Item.h / Item.cpp         # Items del juego
├── Battle.h / Battle.cpp     # Motor de combate con multiplicadores de tipo
├── BattleStrategy.h           # Interfaz Strategy para combate
├── AggressiveStrategy.h / .cpp # Siempre ataca
├── DefensiveStrategy.h / .cpp  # Siempre usa item
├── RandomStrategy.h / .cpp   # Acción aleatoria
├── Event.h                  # Clase base abstracta de eventos
├── WildPokemonEvent.h / .cpp
├── TrainerBattleEvent.h / .cpp
├── GymEvent.h / .cpp
├── PokemonCenterEvent.h / .cpp
├── ShopEvent.h / .cpp
├── TreasureEvent.h / .cpp
├── NpcEvent.h / .cpp
├── FileLoader.h / .cpp       # Carga de archivos de entrada
├── MapGenerator.h / .cpp    # Construye el mundo desde archivo
├── Logger.h / .cpp           # Singleton para bitácora
├── PokemonFactory.h / .cpp   # Factory para crear Pokémon
├── Objective.h / .cpp        # Objetivos de la simulación
├── Obstacle.h / .cpp         # Obstáculos del mundo
├── Enums.h                   # Enumeraciones del dominio
├── Element.h                 # Interfaz abstracta de elementos
├── CMakeLists.txt
└── data/
    ├── especies.txt          # Definición de especies Pokémon
    ├── items.txt             # Items disponibles
    ├── lideres.txt           # Líderes de gimnasio y sus equipos
    ├── mapa.txt              # Mapa del mundo con conexiones
    ├── bitacora.txt          # Generado al ejecutar
    └── reporte_final.txt     # Generado al ejecutar
```

---

## Compilación

### Con CMake (recomendado — CLion)

Desde la raíz del proyecto:

```bash
cmake -S . -B build
cmake --build build
```

### Con g++ directo

```bash
g++ -std=c++20 -o simulacion main.cpp Game.cpp World.cpp Location.cpp Player.cpp \
    Trainer.cpp Pokemon.cpp PokemonSpecies.cpp Inventory.cpp Item.cpp \
    Battle.cpp AggressiveStrategy.cpp DefensiveStrategy.cpp RandomStrategy.cpp \
    BattleStrategy.cpp Event.cpp WildPokemonEvent.cpp TrainerBattleEvent.cpp \
    GymEvent.cpp PokemonCenterEvent.cpp ShopEvent.cpp \
    TreasureEvent.cpp TrapEvent.cpp NpcEvent.cpp FileLoader.cpp MapGenerator.cpp \
    Logger.cpp PokemonFactory.cpp Objective.cpp Obstacle.cpp
```

---

## Ejecución

El ejecutable debe iniciarse **desde la raíz del proyecto** para que las rutas relativas a `data/` funcionen correctamente.

```bash
./build/Proyecto_2
```

Si se compiló con el comando directo:

```bash
./simulacion
```

> **Importante:** La carpeta `data/` debe existir junto al ejecutable con los archivos `especies.txt`, `items.txt`, `lideres.txt` y `mapa.txt`.

---

## Archivos de entrada

### `data/especies.txt`
Define las especies Pokémon disponibles.  
Formato: `nombre,tipo,hpBase,ataqueBase,defensaBase,nivelEvolucion,nombreEvolucion,rareza`

### `data/items.txt`
Define los items del juego.  
Formato: `nombre,tipo,valor`

### `data/lideres.txt`
Define los líderes de gimnasio y sus equipos.  
Formato: `nombre,plataGanada,pokemon1:nivel1,pokemon2:nivel2,...`

### `data/mapa.txt`
Define los lugares del mundo, sus eventos y sus conexiones. El juego construye desde este archivo un recorrido roguelike por capas, con rutas alternativas que se unen antes de cada gimnasio.
Formato: `id,nombre,tipoEvento,conexion1,conexion2,...`

Las líneas que comienzan con `#` son comentarios y se ignoran.

---

## Archivos de salida

| Archivo | Descripción |
|---|---|
| `data/bitacora.txt` | Registro completo de todos los eventos durante la simulación |
| `data/reporte_final.txt` | Estado final del jugador, equipo, medallas y objetivos |

---

## Patrones de diseño utilizados

| Patrón | Clase(s) | Justificación |
|---|---|---|
| Singleton | `Logger` | Una sola instancia de bitácora en todo el sistema |
| Strategy | `BattleStrategy`, `AggressiveStrategy`, `DefensiveStrategy`, `RandomStrategy` | Los entrenadores normales usan `RandomStrategy`, Misty usa `DefensiveStrategy` y los demás líderes usan `AggressiveStrategy` |
| Factory | `PokemonFactory` | Centraliza la creación de instancias Pokémon |
| Polimorfismo | `Event` y sus 7 subclases | Cada lugar ejecuta su evento sin conocer el tipo concreto |

---

## Técnicas de Programación II aplicadas

- Herencia: `Player → Character`, `Location → Space`, `Event → subclases`
- Polimorfismo: jerarquía de `Event`, jerarquía de `BattleStrategy`
- Sobrecarga de operadores: `Pokemon::operator>` compara por nivel de poder
- Manejo de archivos: lectura con `ifstream`, escritura con `ofstream`
- Manejo de excepciones: bloques `try/catch` en `FileLoader` y `MapGenerator`
- Memoria dinámica: `Location*` en `World`, `Event*` en `Game`, destructor en `World`
- Programación genérica: uso de `std::vector<T>` con distintos tipos
