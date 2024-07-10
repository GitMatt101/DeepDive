#pragma once
#include "../lib.h"
#include "mesh.hpp"

/**
* Inizializza il VAO e i VBO di una mesh.
* @param mesh - La mesh da inizializzare.
*/
void initMeshVAO(Mesh* mesh);

/**
* Inizializza il VAO e il VBO del testo.
* @param textVAO - VAO del testo.
* @param textVBO - VBO del testo.
*/
void initTextVAO(GLuint* textVAO, GLuint* textVBO);