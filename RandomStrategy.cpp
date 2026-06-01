//
// Created by Gabri on 5/31/26.
//

#include "RandomStrategy.h"
#include <cstdlib>

// Randomly choose attack (0) or item (1)
int RandomStrategy::elegirAccion() {
    return rand() % 2;
}