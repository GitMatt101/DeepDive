#pragma once
#include "../lib.h"

// Assi di lavoro per le trasformazioni
const enum Axis { X, Y, Z };

// Trasformazioni
const enum Transformation { TRANSLATION, ROTATION, SCALE };

/*
* Gestisce tutti gli eventi con la tastiera.
* 
* @param key - Tasto premuto.
* @param x - X del cursore quando il tasto viene premuto.
* @param y - Y del cursore quando il tasto viene premuto.
*/
void keyboardEvent(unsigned char key, int x, int y);

/*
* Gestisce lo zoom con la rotella del mouse.
*
* @param wheel - Numero della rotella.
* @param direction - Direzione di scorrimento della rotella (+1 avanti, -1 indietro)
* @param x - X del cursore quando la rotella viene usata.
* @param y - Y del cursore quando la rotella viene usata.
*/
void zoom(int wheel, int direction, int x, int y);

/*
* Gestisce tutti gli eventi con il mouse.
* 
* @param button - Pulsante premuto.
* @param x - X del cursore quando avviene un click.
* @param x - X del cursore quando avviene un click.
*/
void mouse(int button, int state, int x, int y);