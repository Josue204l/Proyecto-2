//
// Created by jr156 on 28/5/2026.
//

#include "Logger.h"
#include <iostream>

Logger::Logger() {
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
    std::cout << message << std::endl;
    if (logFile.is_open()) {
        logFile << message << "\n";
        logFile.flush();
    }
}

void Logger::close() {
    if (logFile.is_open()) logFile.close();
}