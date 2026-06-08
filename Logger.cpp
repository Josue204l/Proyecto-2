//
// Created by jr156 on 28/5/2026.
//

#include "Logger.h"
#include <iostream>

Logger::Logger() {
    reiniciar();
}

void Logger::reiniciar() {
    if (logFile.is_open()) logFile.close();
    // La bitácora arranca limpia en cada corrida para no mezclar historias viejas con la actual.
    logFile.open("data/bitacora.txt", std::ios::out | std::ios::trunc);
    if (!logFile.is_open()) {
        std::cerr << "[Logger] No se pudo abrir data/bitacora.txt" << std::endl;
    }
}

Logger& Logger::getInstancia() {
    static Logger instancia;
    return instancia;
}

void Logger::log(const std::string& message) {
    // Se imprime y se guarda de una vez, asi si algo explota igual queda rastro de por qué pasó.
    std::cout << message << std::endl;
    if (logFile.is_open()) {
        logFile << message << "\n";
        logFile.flush();
    }
}

void Logger::close() {
    if (logFile.is_open()) logFile.close();
}
