//
// Created by Gabri on 5/31/26.
//

#ifndef PROYECTO_2_ESPECIEPOKEMON_H
#define PROYECTO_2_ESPECIEPOKEMON_H
#include <string>
#include "Enums.h"
using namespace std;

class EspeciePokemon {
    private:
    string nombre;
    TipoPokemon tipo;
    int hpBase;
    int ataqueBase;
    int defensaBase;
    int nivelEvolucion;
    string nombreEvolucion;
    Rareza rarity;
    public:
    EspeciePokemon();
    EspeciePokemon(const string& nom,TipoPokemon tip,int hpb,int ataBase,int defeBase,int nivEvo, const string& nombEvo,Rareza rar);
    string getNombre()const;
    TipoPokemon getTipo()const;
    int getHpBase()const;
    int getAtaqueBase() const ;
    int getDefenseBase() const ;
    int getNivelEvolucion() const;
    string getNombreEvolucion()  const;
   Rareza getRareza()const ;

};


#endif //PROYECTO_2_ESPECIEPOKEMON_H