#include "../header_files/shaderMaker.h"
#include <iostream>
#include <fstream>

#pragma warning(disable:4996)
char* ShaderMaker::readShaderSource(const char* shaderFile)
{
	FILE* fp = fopen(shaderFile, "rb");
	if (fp == NULL) return NULL;

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buffer = new char[size + 1];
	fread(buffer, 1, size, fp);

	buffer[size] = '\0';
	fclose(fp);

	return buffer;
}

GLuint ShaderMaker::createProgram(char* vertexFile, char* fragmentFile)
{
	int success;
	char infoLog[512];

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Read vertex shader code
	GLchar* vertexShader = readShaderSource(vertexFile);
	// Create an ID for vertex shader
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	// Link vertex shader code to its ID
	glShaderSource(vertexShaderID, 1, (const char**)&vertexShader, NULL);
	// Compile vertex shader
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		fprintf(stderr, "VERTEX SHADER ERROR\n%s\n", infoLog);
	}

	// Read fragment shader code
	const GLchar* FragmentShader = readShaderSource(fragmentFile);
	// Create an ID for fragment shader
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	// Link fragment shader code to its ID
	glShaderSource(fragmentShaderID, 1, (const char**)&FragmentShader, NULL);
	// Compile fragment shader
	glCompileShader(fragmentShaderID);
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		fprintf(stderr, "FRAGMENT SHADER ERROR\n%s\n", infoLog);
	}

	if (glGetError() != GL_NO_ERROR) exit(-1);

	// Create an ID for the whole program
	GLuint programId = glCreateProgram();

	glAttachShader(programId, vertexShaderID);
	glAttachShader(programId, fragmentShaderID);
	glLinkProgram(programId);

	return programId;
}
