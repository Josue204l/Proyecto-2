//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_LUGAR_H
#define PROYECTO_2_LUGAR_H
#include <vector>
#include "Enums.h"

class Lugar {
private:
    int id;
    TipoEvento tipo;
    std::vector<int> siguienteLugar;
    public:
    Lugar();
    Lugar(int id, TipoEvento tipo);
    int getId()const ;
    TipoEvento getTipoEvento() const ;
    void agregarConexion(int idLugar);
    const std::vector<int>& getSiguienteLugar() const;
};


#endif //PROYECTO_2_LUGAR_H