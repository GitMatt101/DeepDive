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

	// Legge il codice del vertex shader
	GLchar* vertexShader = readShaderSource(vertexFile);
	// Crea un id per il vertex shader
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	// Collega il codice all'id
	glShaderSource(vertexShaderID, 1, (const char**)&vertexShader, NULL);
	// Compila il codice
	glCompileShader(vertexShaderID);
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		fprintf(stderr, "ERRORE DI COMPILAZIONE NEL VERTEX SHADER\n%s\n", infoLog);
	}

	// Legge il codice del fragment shader
	const GLchar* FragmentShader = readShaderSource(fragmentFile);
	// Crea un id per il fragment shader
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	// Collega il codice all'id
	glShaderSource(fragmentShaderID, 1, (const char**)&FragmentShader, NULL);
	// Compila il codice
	glCompileShader(fragmentShaderID);
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		fprintf(stderr, "ERRORE DI COMPILAZIONE NEL FRAGMENT SHADER\n%s\n", infoLog);
	}

	if (glGetError() != GL_NO_ERROR) exit(-1);

	// Create l'id per il programma che utilizzerà gli shader forniti
	GLuint programId = glCreateProgram();

	glAttachShader(programId, vertexShaderID);
	glAttachShader(programId, fragmentShaderID);
	glLinkProgram(programId);

	return programId;
}
