#include "Pokemon.h"
#include "Logger.h"

// Constructor por defecto, crea un pokemon vacio
Pokemon::Pokemon() : nivel(1), experiencia(0), hpActual(1) {
}

// Constructor principal: recibe la especie y el nivel inicial
Pokemon::Pokemon(const EspeciePokemon& especie, int nivel)
    : especie(especie), nivel(nivel), experiencia(0) {
    hpActual = getHPMAX(); // empieza con HP lleno
}

EspeciePokemon Pokemon::getEspeciePokemon() const { return especie; }
string Pokemon::getNombre() const { return especie.getNombre(); }
TipoPokemon Pokemon::getTipoPokemon() const { return especie.getTipo(); }
int Pokemon::getNivel() const { return nivel; }
int Pokemon::getExperiencia() const { return experiencia; }
int Pokemon::getHpActual() const { return hpActual; }

// HP maximo sube con el nivel
int Pokemon::getHPMAX() const {
    return especie.getHpBase() + (nivel * 3);
}

// Ataque sube con el nivel
int Pokemon::getAtaque() const {
    return especie.getAtaqueBase() + (nivel * 2);
}

// Defensa sube con el nivel
int Pokemon::getDefensa() const {
    return especie.getDefenseBase() + (nivel * 2);
}

bool Pokemon::estaMuerto() const {
    return hpActual <= 0;
}

// Recibe danno y no baja de 0
void Pokemon::dannioRecibido(int dannio) {
    hpActual -= dannio;
    if (hpActual < 0) {
        hpActual = 0;
    }
}

// Cura sin pasar el maximo
void Pokemon::curar(int cantidad) {
    hpActual += cantidad;
    if (hpActual > getHPMAX()) {
        hpActual = getHPMAX();
    }
}

// Cura completamente
void Pokemon::curarFull() {
    hpActual = getHPMAX();
}

// Gana experiencia y sube de nivel si llega al umbral
void Pokemon::ganarExperiencia(int cantidad) {
    experiencia += cantidad;
    // sube de nivel cada vez que acumula nivel*100 de experiencia
    while (experiencia >= nivel * 100) {
        experiencia -= nivel * 100;
        subirNivel();
    }
}
void Pokemon::subirNivelesDirecto(int n) {
    for (int i = 0; i < n; i++)
        subirNivel();
}
void Pokemon::subirNivel() {

    nivel++;

    hpActual = getHPMAX();

    Logger::getInstancia().log(
        getNombre() +
        " subio al nivel " +
        std::to_string(nivel)
    );
}
// Verifica si puede evolucionar segun su nivel actual
bool Pokemon::puedeEvolucionar() const {
    if (especie.getNombreEvolucion() == "NADA") {
        return false;
    }
    return nivel >= especie.getNivelEvolucion();
}

// Cambia la especie del pokemon por su evolucion
void Pokemon::evolucionar(const EspeciePokemon& nuevaEspecie) {
    especie = nuevaEspecie;
    hpActual = getHPMAX();
}

// El poder total es ataque + defensa + hp maximo
int Pokemon::getNivelPoder() const {
    return getAtaque() + getDefensa() + getHPMAX();
}

bool Pokemon::operator>(const Pokemon& otro) const {
    return getNivelPoder() > otro.getNivelPoder();
}
