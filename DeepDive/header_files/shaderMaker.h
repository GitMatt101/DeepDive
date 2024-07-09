#pragma once
#include "../lib.h"

// Class used to setup the program and read the shaders.
class ShaderMaker
{
	public:
		/*
		* Creates the program ID.
		* 
		* @param vertexFile -  Vertex shader file path.
		* @param fragmentFile - Fragment shader file path.
		* @return The program's ID.
		*/
		static GLuint createProgram(char* vertexFile, char* fragmentFile);

		/*
		* Reads the shader information.
		* 
		* @param shaderFile - Shader file path
		*/
		static char* readShaderSource(const char* shaderFile);

	private:
		// Empty constructor
		ShaderMaker() {}
};