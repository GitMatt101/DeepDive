#include <iostream>
#include "lib.h"
#include "header_files/mesh.hpp"
#include "header_files/view.h"
#include "header_files/interactions.h"
#include "header_files/text.h"
#include "header_files/init.h"
#include "header_files/VAO_manager.h"
#include "header_files/light_materials.hpp"
#include "header_files/camera.h"
#include "header_files/menu.hpp"
#pragma warning(disable : 4996)
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Vettori per la gestione della scena
pair<vector<Mesh*>, vector<vector<Mesh*>>> scene;
pair<vector<Mesh*>, vector<Mesh*>> sharks;
vector<Mesh*> bubbles;
Mesh* skybox;

// Oggetti per la navigazione della scena
View camera;
Perspective cameraPerspective;

// Parametri per la gestione delle mesh
vector<Material> materials;
vector<Shader> shaders;
LightPoint light;

// Matrici
mat4 projectionMatrix;
mat4 projectionMatrix_text;
mat4 view;

// Id dei programmi
unsigned int programId;
unsigned int programId_text;
unsigned int programId_cubemap;
unsigned int programId_reflection;

// Parametri uniform per gestire le variabili dentro ai file shader
unsigned int uniformProjectionMatrix, uniformProjectionMatrixCubeMap, uniformProjectionMatrixReflection;
unsigned int uniformViewMatrix, uniformViewMatrixCubeMap, uniformViewMatrixReflection;
unsigned int uniformModelMatrix, uniformModelMatrixReflection;
unsigned int uniformViewPosition, uniformViewPositionReflection;
unsigned int uniformShader;
unsigned int uniformTime;
unsigned int uniformTexture;
unsigned int uniformCubeMapReflection;
LightShaderUniform uniformLight = {};

// VAO e VBO per testo e skybox
GLuint textVAO;
GLuint textVBO;
GLuint skyboxVAO;
GLuint skyboxVBO;
GLuint skyboxEBO;

unsigned int cubeMapTexture;

// Coordinate del mouse utilizzate per muovere la telecamera
int mouseX;
int mouseY;

// Angoli utilizzati per lil movimento della luce e della telecamera
float lightAngle = 0.0f;
float Theta = -90.0f;
float Phi = 0.0f;

// Stringhe utilizzate per gestire le trasformazioni
string transformation = "Translation";
string workingAxis = "X";

void drawScene(void) {
	// Imposta la vista
	projectionMatrix_text = ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);
	projectionMatrix = perspective(radians(cameraPerspective.fov), cameraPerspective.aspect, cameraPerspective.nearPlane, cameraPerspective.farPlane);
	view = lookAt(vec3(camera.position), vec3(camera.target), vec3(camera.upVector));

	// Resetta il colore di sfondo
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Attiva la skybox
	glDepthMask(GL_FALSE);
	glUseProgram(programId_cubemap);
	glUniform1i(glGetUniformLocation(programId_cubemap, "skybox"), 0);
	glUniformMatrix4fv(uniformProjectionMatrixCubeMap, 1, GL_FALSE, value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformViewMatrixCubeMap, 1, GL_FALSE, value_ptr(view));
	glBindVertexArray(*skybox->getVAO());
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
	glDrawElements(GL_TRIANGLES, skybox->getIndices()->size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	// Renderizza le bolle utilizzando lo shader per la riflessione della skybox
	glUseProgram(programId_reflection);
	for (Mesh* bubble : bubbles) {
		bubble->setAnchorWorld(*bubble->getAnchorObj());
		bubble->setAnchorWorld(*bubble->getModel() * *bubble->getAnchorWorld());
		glUniformMatrix4fv(uniformProjectionMatrixReflection, 1, GL_FALSE, value_ptr(projectionMatrix));
		glUniformMatrix4fv(uniformModelMatrixReflection, 1, GL_FALSE, value_ptr(*bubble->getModel()));
		glUniformMatrix4fv(uniformViewMatrixReflection, 1, GL_FALSE, value_ptr(view));
		glUniform3f(uniformViewPositionReflection, camera.position.x, camera.position.y, camera.position.z);
		glBindVertexArray(*bubble->getVAO());
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		glDrawElements(GL_TRIANGLES, (bubble->getIndices()->size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	// Imposta l'id del programma per utilizzare le shader per le mesh
	glUseProgram(programId);
	glPointSize(10.0f);

	glUniformMatrix4fv(uniformProjectionMatrix, 1, GL_FALSE, value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, value_ptr(view));

	glUniform3f(uniformLight.light_position_pointer, light.position.x + 80 * cos(radians(lightAngle)), light.position.y, light.position.z + 80 * sin(radians(lightAngle)));
	glUniform3f(uniformLight.light_color_pointer, light.color.r, light.color.g, light.color.b);
	glUniform1f(uniformLight.light_power_pointer, light.power);
	glUniform3f(uniformViewPosition, camera.position.x, camera.position.y, camera.position.z);
	glUniform1f(uniformTime, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

	for (Mesh* mesh : scene.first) {
		mesh->setAnchorWorld(*mesh->getAnchorObj());
		mesh->setAnchorWorld(*mesh->getModel() * *mesh->getAnchorWorld());

		glUniform1i(uniformShader, mesh->getShader().value);
		glUniform3fv(uniformLight.material_ambient, 1, value_ptr(materials[mesh->getMaterialType()].ambient));
		glUniform3fv(uniformLight.material_diffuse, 1, value_ptr(materials[mesh->getMaterialType()].diffuse));
		glUniform3fv(uniformLight.material_specular, 1, value_ptr(materials[mesh->getMaterialType()].specular));
		glUniform1f(uniformLight.material_shininess, materials[mesh->getMaterialType()].shininess);

		// Se la texture � attiva allora la applica sulla mesh
		if (mesh->isUseTextureActive()) {
			glUniform1i(uniformTexture, 0);
			glBindTexture(GL_TEXTURE_2D, mesh->getTexture());
		}
		
		glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, value_ptr(*mesh->getModel()));
		glBindVertexArray(*mesh->getVAO());

		glDrawElements(GL_TRIANGLES, (mesh->getIndices()->size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	for (vector<Mesh*> mesh : scene.second) {
		for (Mesh* subMesh : mesh) {
			subMesh->setAnchorWorld(*subMesh->getAnchorObj());
			subMesh->setAnchorWorld(*subMesh->getModel() * *subMesh->getAnchorWorld());

			glUniform1i(uniformShader, subMesh->getShader().value);
			glUniform3fv(uniformLight.material_ambient, 1, value_ptr(subMesh->getMaterial().ambient));
			glUniform3fv(uniformLight.material_diffuse, 1, value_ptr(subMesh->getMaterial().diffuse));
			glUniform3fv(uniformLight.material_specular, 1, value_ptr(subMesh->getMaterial().specular));
			glUniform1f(uniformLight.material_shininess, subMesh->getMaterial().shininess);

			// Se la texture � attiva allora la applica sulla mesh
			if (subMesh->isUseTextureActive()) {
				glUniform1i(uniformTexture, 0);
				glBindTexture(GL_TEXTURE_2D, subMesh->getTexture());
			}

			glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, value_ptr(*subMesh->getModel()));
			glBindVertexArray(*subMesh->getVAO());

			glDrawElements(GL_TRIANGLES, (subMesh->getIndices()->size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	// Renderizza il testo per le trasformazioni
	renderText(programId_text, projectionMatrix_text, transformation, textVAO, textVBO, 10.0f, (float)HEIGHT - 30.0f, 0.5f, vec3(1.0f, 0.0f, 1.0f));
	renderText(programId_text, projectionMatrix_text, workingAxis, textVAO, textVBO, 10.0f, (float)HEIGHT - 60.0f, 0.5f, vec3(1.0f, 0.0f, 1.0f));

	// Renderizza il testo per gli oggetti cliccati
	for (Mesh* mesh : scene.first) {
		if (mesh->isSelected()) {
			renderText(programId_text, projectionMatrix_text, "Oggetto Selezionato:", textVAO, textVBO, 10.0f, (float)HEIGHT - 90.0f, 0.5f, vec3(1.0f, 0.0f, 0.2f));
			renderText(programId_text, projectionMatrix_text, mesh->getName(), textVAO, textVBO, 250.0f, (float)HEIGHT - 90.0f, 0.5f, vec3(1.0f, 0.0f, 0.2f));
		}
	}
	for (vector<Mesh*> mesh : scene.second) {
		for (Mesh* subMesh : mesh) {
			if (subMesh->isSelected()) {
				renderText(programId_text, projectionMatrix_text, "Oggetto Selezionato:", textVAO, textVBO, 10.0f, (float)HEIGHT - 90.0f, 0.5f, vec3(1.0f, 0.0f, 0.2f));
				renderText(programId_text, projectionMatrix_text, subMesh->getName(), textVAO, textVBO, 250.0f, (float)HEIGHT - 90.0f, 0.5f, vec3(1.0f, 0.0f, 0.2f));
			}
		}
	}
	for (Mesh* bubble : bubbles) {
		if (bubble->isSelected()) {
			renderText(programId_text, projectionMatrix_text, "Oggetto Selezionato:", textVAO, textVBO, 10.0f, (float)HEIGHT - 90.0f, 0.5f, vec3(1.0f, 0.0f, 0.2f));
			renderText(programId_text, projectionMatrix_text, bubble->getName(), textVAO, textVBO, 250.0f, (float)HEIGHT - 90.0f, 0.5f, vec3(1.0f, 0.0f, 0.2f));
		}
	}
	glutSwapBuffers();
}

void update(int value)
{
	// Movimento degli squali
	float seconds = 5.0f;
	float angle = 360.0f / ((float)FPS * 5.0f);
	float circumference = 2.0f * (float)PI * 200.0f;
	float movement = circumference / ((float)FPS * seconds);
	for (Mesh* mesh : sharks.first) {
		mesh->setModel(rotate(*mesh->getModel(), radians(angle), vec3(0.0f, 0.0f, 1.0f)));
		mesh->setModel(translate(*mesh->getModel(), vec3(1.0f, 0.0f, 0.0f) * movement));
	}
	for (Mesh* mesh : sharks.second) {
		mesh->setModel(rotate(*mesh->getModel(), radians(angle), vec3(0.0f, 0.0f, 1.0f)));
		mesh->setModel(translate(*mesh->getModel(), vec3(1.0f, 0.0f, 0.0f) * movement));
	}

	// Movimento delle bolle
	float bubbleMovement = 0.1f;
	static float currentPosition = 0.0f;
	static float cyclic = 0.0f;
	cyclic += 3.0f;
	currentPosition += bubbleMovement * 0.1f;
	if (currentPosition >= 5.0f) {
		bubbleMovement = -currentPosition * 10.0f;
		currentPosition = 0.0f;
	}
	for (Mesh* bubble : bubbles)
		bubble->setModel(translate(*bubble->getModel(), vec3(cos(radians(cyclic)) * 0.1f, bubbleMovement, sin(radians(cyclic)) * 0.1f)));

	lightAngle += 1.0f;
	glutTimerFunc(1000.0f / (float)FPS, update, 0);
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("3D Scene");

	glutDisplayFunc(drawScene);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboardEvent);
	glutMouseWheelFunc(zoom);
	glutPassiveMotionFunc(passiveCameraMovement);
	glutTimerFunc(100, update, 0);

	glewExperimental = GL_TRUE;

	glewInit();
	initShader();
	initMaterials();
	initShaders();
	initCubeMap();
	initCamera();
	initScene();
	initTextVAO(&textVAO, &textVBO);
	initFreetype();
	buildOpenGLMenu();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Parametri per le mesh
	uniformProjectionMatrix = glGetUniformLocation(programId, "Projection");
	uniformModelMatrix = glGetUniformLocation(programId, "Model");
	uniformViewMatrix = glGetUniformLocation(programId, "View");
	uniformShader = glGetUniformLocation(programId, "shader");
	uniformTime = glGetUniformLocation(programId, "time");
	uniformViewPosition = glGetUniformLocation(programId, "viewPos");
	uniformTexture = glGetUniformLocation(programId, "tex");
	uniformLight.light_position_pointer = glGetUniformLocation(programId, "light.position");
	uniformLight.light_color_pointer = glGetUniformLocation(programId, "light.color");
	uniformLight.light_power_pointer = glGetUniformLocation(programId, "light.power");
	uniformLight.material_ambient = glGetUniformLocation(programId, "material.ambient");
	uniformLight.material_diffuse = glGetUniformLocation(programId, "material.diffuse");
	uniformLight.material_specular = glGetUniformLocation(programId, "material.specular");
	uniformLight.material_shininess = glGetUniformLocation(programId, "material.shininess");

	// Parametri per la skybox
	uniformProjectionMatrixCubeMap = glGetUniformLocation(programId_cubemap, "Projection");
	uniformViewMatrixCubeMap = glGetUniformLocation(programId_cubemap, "View");

	// Parametri per la riflessione
	uniformModelMatrixReflection = glGetUniformLocation(programId_reflection, "Model");
	uniformViewMatrixReflection = glGetUniformLocation(programId_reflection, "View");
	uniformProjectionMatrixReflection = glGetUniformLocation(programId_reflection, "Projection");
	uniformViewPositionReflection = glGetUniformLocation(programId_reflection, "viewPos");
	uniformCubeMapReflection = glGetUniformLocation(programId_reflection, "cubemap");

	glutMainLoop();
}