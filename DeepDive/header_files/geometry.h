#pragma once
#include "../lib.h"

// Struttura usata per rappresentare un vertice.
struct Vertex {
	vec3 coordinates;	// Coordinate del vertice
	vec4 color;			// Colore del vertice

	Vertex(const vec3& coordinates, const vec4& color) : coordinates(coordinates), color(color) {
		this->coordinates = coordinates;
		this->color = color;
	}

	Vertex() {
		this->coordinates = vec3(0.0f, 0.0f, 0.0f);
		this->color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
};

/**
* Crea un parallelepipedo caricando i dati nei vettori passati come parametri.
* @param width - Larghezza.
* @param height - Altezza.
* @param depth - Spessore/Profondità.
* @param color1 - Colore primario.
* @param color2 - Colore secondario.
* @param vertices - Vettore dove vengono caricati i vertici.
* @param indices - Vettore dove vengono caricati gli indici.
*/
void createCuboid(float width, float height, float depth, vec4 color1, vec4 color2, vector<Vertex>* vertices, vector<GLuint>* indices);

/**
* Crea un'ellisse caricando i dati nei vettori passati come parametri.
* @param rx - Raggio X.
* @param ry - Raggio Y.
* @param rz - Raggio Z.
* @param precision - Precisione: più è alta, più sarà definita l'ellisse.
* @param color - Colore.
* @param vertices - Vettore dove vengono caricati i vertici.
* @param indices - Vettore dove vengono caricati gli indici.
* @param normals - Vettore dove vengono caricati i vettori normali.
* @param textureCoordinates - Vettore dove vengono caricate le coordinate di texture.
*/
void createEllipsoid(float rx, float ry, float rz, int precision, vec4 color, vector<Vertex>* vertices, vector<GLuint>* indices, vector<vec3>* normals, vector<vec2>* textureCoordinates);

/**
* Crea un pannello 2D caricando i dati nei vettori passati come parametri.
* @param width - Larghezza.
* @param depth - Spessore/Profondità.
* @param repeat - Quante volte deve essere ripetuta la texture.
* @param color - Colore.
* @param vertices - Vettore dove vengono caricati i vertici.
* @param indices - Vettore dove vengono caricati gli indici.
* @param normals - Vettore dove vengono caricati i vettori normali.
* @param textureCoordinates - Vettore dove vengono caricate le coordinate di texture.
*/
void createPanel(float width, float depth, float repeat, vec4 color, vector<Vertex>* vertices, vector<GLuint>* indices, vector<vec3>* normals, vector<vec2>* textureCoordinates);