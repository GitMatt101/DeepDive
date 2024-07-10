#pragma once
#include "../lib.h"

/**
* Carica una texture.
* @param path - Percorso del file della texture.
* @param bool - Parametro per invertire la texture.
* @return Id della texture caricata in memoria.
*/
unsigned int loadTexture(char const* path, bool flip);

/**
* Carica una cubemap per la skybox.
* @param faces - Percorsi dei file con le facce della cubemap.
* @return Id della cubemap caricata in memoria.
*/
unsigned int loadCubeMap(vector<string> faces);