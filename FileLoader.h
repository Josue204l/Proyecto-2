//
// Created by jr156 on 28/5/2026.
//

#ifndef PROYECTO_2_FILELOADER_H
#define PROYECTO_2_FILELOADER_H
#include <vector>
#include "PokemonSpecies.h"
#include "Item.h"
#include "Trainer.h"


class FileLoader {
    private:
    std::vector<PokemonSpecies> especie;
    std::vector<Item> items;
    std::vector<Trainer> liderGym;
    public:
    FileLoader();
    bool cargarEspecies(const std::string& nombreArchivo);
    bool cargarItem(const std::string& nombreArchivo);
    bool cargarLiderGym(const std::string& nombreArchivo);
    const std::vector<PokemonSpecies>& getEspecie() const;
    const std::vector<Item>& getItems() const;
    const std::vector<Trainer>& getLiderGym() const;
    void setStarter(bool valor);
};


#endif //PROYECTO_2_FILELOADER_H