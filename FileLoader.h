//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_FILELOADER_H
#define PROYECTO_2_FILELOADER_H
#include <vector>
#include "EspeciePokemon.h"
#include "Item.h"
#include "Entrenador.h"


class FileLoader {
    private:
    std::vector<EspeciePokemon> especie;
    std::vector<Item> items;
    std::vector<Entrenador> liderGym;
    public:
    FileLoader();
    bool cargarEspecies(const std::string& nombreArchivo);
    bool cargarItem(const std::string& nombreArchivo);
    bool cargarLiderGym(const std::string& nombreArchivo);
    const std::vector<EspeciePokemon>& getEspecie() const;
    const std::vector<Item>& getItems() const;
    const std::vector<Entrenador>& getLiderGym() const;
};


#endif //PROYECTO_2_FILELOADER_H