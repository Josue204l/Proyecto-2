#ifndef PROYECTO_2_SPACE_H
#define PROYECTO_2_SPACE_H
#include <string>
#include <vector>
using namespace std;

// Clase base abstracta para cualquier lugar del mundo
// Lugar hereda de esta clase
class Space {
public:
    virtual ~Space() = default;
    virtual int getId() const = 0;
    virtual string getNombre() const = 0;
    virtual const vector<int>& getConexiones() const = 0; // IDs de lugares conectados
};

#endif // PROYECTO_2_SPACE_H
