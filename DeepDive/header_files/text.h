#pragma once
#include "../lib.h"

// Inizializza la libreria Freetype
void initFreetype(void);

/*
* Visualizza del testo.
* 
* @param programId_text - Id del programma di testo.
* @param projectionMatrix_text - Matrice di proiezione del testo.
* @param text - Testo da visualizzare.
* @param textVAO - VAO del testo.
* @param textVBO - VBO del testo.
* @param x - X della posizione del testo.
* @param y - Y della posizione del testo.
* @param scale - Dimensione del testo in pixel.
* @param color - Colore del testo.
*/
void renderText(int programId_text, mat4 projectionMatrix_text, string text, unsigned int textVAO, unsigned int textVBO, float x, float y, float scale, vec3 color);