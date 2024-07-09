#pragma once
#include "../lib.h"

//Initializes Freetype library, setting up how the text has to be visualized.
void initFreetype(void);

/*
* Visualizes the given text on the screen.
* 
* @param programId_text - ID of the program used to generate text.
* @param projectionMatrix_text - Projection matrix used to create and transform text.
* @param text - Text to visualize.
* @param textVAO - VAO of the text.
* @param textVBO - VBO of the text.
* @param x - X value of the position where the text is going to be visualized.
* @param y - Y value of the position where the text is going to be visualized.
* @param scale - Size of the text in pixels.
* @param color - Color of the text
*/
void renderText(int programId_text, mat4 projectionMatrix_text, string text, unsigned int textVAO, unsigned int textVBO, float x, float y, float scale, vec3 color);
