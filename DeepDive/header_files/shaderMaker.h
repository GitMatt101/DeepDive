#pragma once
#include "../lib.h"

class ShaderMaker
{
	public:
		/*
		* Creates l'id del programma di shader.
		* 
		* @param vertexFile -  File del vertex shader.
		* @param fragmentFile - File del fragment shader.
		* @return L'id del programma.
		*/
		static GLuint createProgram(char* vertexFile, char* fragmentFile);

		/*
		* Legge il codice di un file shader.
		* 
		* @param shaderFile - Percorso del file shader
		*/
		static char* readShaderSource(const char* shaderFile);

	private:
		ShaderMaker() {}
};