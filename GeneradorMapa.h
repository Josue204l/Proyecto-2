//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_GENERADORMAPA_H
#define PROYECTO_2_GENERADORMAPA_H
#include <string>
#include "World.h"
#include <cstdlib>
#include <vector>
class GeneradorMapa {
public:
    static World generarMundo(const std::string& archivoMapa);
    static World generarMundoAleatorio(int capas = 10);

};

#endif //PROYECTO_2_GENERADORMAPA_H