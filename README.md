# Motor de Simulación Pokémon
**Curso:** EIF204 - Programación II  
**Carrera:** Ingeniería de Sistemas de Información  
**Universidad Nacional de Costa Rica**  
**Periodo:** I Ciclo 2026

---

## Descripción de la simulación

El sistema simula una aventura Pokémon completa en consola. Un jugador llamado **Ash** recorre un mundo compuesto por 29 lugares interconectados, enfrentando Pokémon salvajes, entrenadores, líderes de gimnasio y eventos especiales. La simulación es automática: el programa toma decisiones por el jugador usando estrategias programadas.

El objetivo es obtener **3 medallas de gimnasio** y sobrevivir hasta llegar a la **Liga Pokémon**.

---

## Estructura del proyecto

```
Proyecto#2/
├── main.cpp                  # Punto de entrada
├── Game.h / Game.cpp         # Motor principal de simulación
├── World.h / World.cpp       # Contenedor del mundo
├── Lugar.h / Lugar.cpp       # Espacio navegable (hereda Space)
├── Space.h                   # Interfaz abstracta de espacio
├── Character.h               # Interfaz abstracta de personaje
├── Jugador.h / Jugador.cpp   # Jugador principal (hereda Character)
├── Entrenador.h / Entrenador.cpp
├── Pokemon.h / Pokemon.cpp   # Entidad Pokémon con stats y evolución
├── EspeciePokemon.h / .cpp   # Datos base de cada especie
├── Inventario.h / .cpp       # Inventario del jugador
├── Item.h / Item.cpp         # Items del juego
├── Pelea.h / Pelea.cpp       # Motor de combate con multiplicadores de tipo
├── PeleaStrategy.h           # Interfaz Strategy para combate
├── AgresivaStrategy.h / .cpp # Siempre ataca
├── DefensaStrategy.h / .cpp  # Siempre usa item
├── RandomStrategy.h / .cpp   # Acción aleatoria
├── Evento.h                  # Clase base abstracta de eventos
├── EventoPokemonSalvaje.h / .cpp
├── EventoBatallaEntrenador.h / .cpp
├── EventoGimnasio.h / .cpp
├── EventoCentroPokemon.h / .cpp
├── EventoTiendaObjetos.h / .cpp
├── EventoTesoro.h / .cpp
├── EventoNPC.h / .cpp
├── FileLoader.h / .cpp       # Carga de archivos de entrada
├── GeneradorMapa.h / .cpp    # Construye el mundo desde archivo
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

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Con g++ directo

```bash
g++ -std=c++20 -o simulacion main.cpp Game.cpp World.cpp Lugar.cpp Jugador.cpp \
    Entrenador.cpp Pokemon.cpp EspeciePokemon.cpp Inventario.cpp Item.cpp \
    Pelea.cpp AgresivaStrategy.cpp DefensaStrategy.cpp RandomStrategy.cpp \
    PeleaStrategy.cpp Evento.cpp EventoPokemonSalvaje.cpp EventoBatallaEntrenador.cpp \
    EventoGimnasio.cpp EventoCentroPokemon.cpp EventoTiendaObjetos.cpp \
    EventoTesoro.cpp EventoNPC.cpp FileLoader.cpp GeneradorMapa.cpp Logger.cpp \
    PokemonFactory.cpp Objective.cpp Obstacle.cpp Character.cpp Space.cpp
```

---

## Ejecución

El ejecutable debe correrse **desde la raíz del proyecto** para que las rutas relativas a `data/` funcionen correctamente.

```bash
./simulacion
```

o en Windows:

```bash
simulacion.exe
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
Define los lugares del mundo y sus conexiones.  
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
| Strategy | `PeleaStrategy`, `AgresivaStrategy`, `DefensaStrategy`, `RandomStrategy` | Permite cambiar el comportamiento de combate sin modificar `Pelea` |
| Factory | `PokemonFactory` | Centraliza la creación de instancias Pokémon |
| Polimorfismo | `Evento` y sus 7 subclases | Cada lugar ejecuta su evento sin conocer el tipo concreto |

---

## Técnicas de Programación II aplicadas

- Herencia: `Jugador → Character`, `Lugar → Space`, `Evento → subclases`
- Polimorfismo: jerarquía de `Evento`, jerarquía de `PeleaStrategy`
- Sobrecarga de operadores: `Pokemon::operator>` compara por nivel de poder
- Manejo de archivos: lectura con `ifstream`, escritura con `ofstream`
- Manejo de excepciones: bloques `try/catch` en `FileLoader` y `GeneradorMapa`
- Memoria dinámica: `Lugar*` en `World`, `Evento*` en `Game`, destructor en `World`
- Programación genérica: uso de `std::vector<T>` con distintos tipos
