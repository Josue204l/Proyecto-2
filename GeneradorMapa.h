//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_GENERADORMAPA_H
#define PROYECTO_2_GENERADORMAPA_H
#include <string>
#include "World.h"

class GeneradorMapa {
public:
    static World generarMundo(const std::string& archivoMapa);
};

#endif //PROYECTO_2_GENERADORMAPA_H