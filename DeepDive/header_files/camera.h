#pragma once

// The speed at which the camera moves.
#define CAMERA_SPEED 0.5f

// Moves the camera forward (+z).
void moveCameraForward();

// Moves the camera backward (-z).
void moveCameraBackward();

// Moves the camera to the right (+x).
void moveCameraRight();

// Moves the camera to the left (-x).
void moveCameraLeft();

// Moves the camera upward (+y).
void moveCameraUpward();

// Moves the camera downward (-y).
void moveCameraDownward();

void passiveCameraMovement(int x, int y);