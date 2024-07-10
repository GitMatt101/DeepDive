#pragma once

// Velocità di movimento della telecamera
#define CAMERA_SPEED 0.5f

// Muove la telecamera in avanti
void moveCameraForward();

// Muove la telecamera indietro
void moveCameraBackward();

// Muove la telecamera a destra
void moveCameraRight();

// Muove la telecamera a sinistra
void moveCameraLeft();

// Muove la telecamera in alto
void moveCameraUpward();

// Muove la telecamera in basso
void moveCameraDownward();

/**
* Funzione Glut per guardare la scena muovendo il mouse.
* @param x - X del cursore.
* @param y - Y del cursore.
*/
void passiveCameraMovement(int x, int y);