//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_LUGAR_H
#define PROYECTO_2_LUGAR_H
#include <vector>
#include <string>
#include "Space.h"
#include "Enums.h"

class Lugar : public Space {
private:
    int id;
    std::string nombre;
    TipoEvento tipo;
    std::vector<int> siguienteLugar;
public:
    Lugar();
    Lugar(int id, const std::string& nombre, TipoEvento tipo);
    int getId() const override;
    std::string getNombre() const override;
    TipoEvento getTipoEvento() const;
    void agregarConexion(int idLugar);
    const std::vector<int>& getSiguienteLugar() const;
    const std::vector<int>& getConexiones() const override;
};

#endif //PROYECTO_2_LUGAR_H