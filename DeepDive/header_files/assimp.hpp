#pragma once
#include "../lib.h"
#include "mesh.hpp"

/**
* Carica una mesh composta da più parti.
* @param path - Percorso del file .obj.
* @param mesh - vettore di mesh che conterrà la figura.
* @param name - nome della figura attribuito a tutte le parti che la compongono.
*/
void loadAssImp(const char* path, vector<Mesh*>* mesh, string name);