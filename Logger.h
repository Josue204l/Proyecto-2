//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_LOGGER_H
#define PROYECTO_2_LOGGER_H
#include <fstream>
#include <string>

class Logger {
    private:
    std::ofstream logFile;
    Logger();
    public:
    static Logger &getInstancia();
    void log(const std::string &message);
    void close();
    Logger(const Logger &) = delete;
    void operator=(const Logger &) = delete;

};


#endif //PROYECTO_2_LOGGER_H