#include "../lib.h"
#include "../header_files/interactions.h"
#include "../header_files/mesh.hpp"
#include "../header_files/view.h"
#include "../header_files/camera.h"
#include <iostream>
using namespace std;

extern pair<vector<Mesh*>, vector<vector<Mesh*>>> scene;
extern vector<Mesh*> bubbles;
extern View camera;
extern Perspective cameraPerspective;
extern mat4 projectionMatrix;
extern mat4 view;
extern string transformation;
extern string workingAxis;
Axis currentAxis = X;
Transformation currentTransformation = TRANSLATION;

extern bool movingTrackBall;
extern int mouseX;
extern int mouseY;

/**
* Calcola il raggio che congiunge la telecamera e la posizione del cursore nello spazio.
* @param x - X del cursore.
* @param y - Y del cursore.
* @return Il raggio.
*/
vec3 getRay(int x, int y);

/**
* Calcola la distanza di intersezione tra una mesh e il raggio di click.
* @param origin - Posizione della telecamera.
* @param direction - Raggio di click.
* @param mesh - Mesh da controllare per l'intersezione.
* @return La distanza tra il click e la mesh.
*/
double getIntersectionDistance(vec3 origin, vec3 direction, Mesh* mesh);

/**
* Modifica la matrice Model di una mesh.
* @param mesh - La mesh da modificare.
* @param translationVector - Vettore di traslazione.
* @param rotationVector - Vettore di rotazione.
* @param rotationAngle - Angolo di rotazione.
* @param scaleValue - Valore di scalatura.
*/
void modifyModelMatrix(Mesh* mesh, vec3 translationVector, vec3 rotationVector, GLfloat rotationAngle, GLfloat scaleValue);

/**
* Applica la trasformazione selezionata a una mesh.
* @param mesh - La mesh da modificare.
* @param transformationValue - Valore di trasformazione.
*/
void applyTransformation(Mesh* mesh, float transformationValue);

void keyboardEvent(unsigned char key, int x, int y) {
	Mesh* selectedMesh = NULL;
	vector<Mesh*> selectedMeshes;

	// Controlla se c'è una mesh selezionata
	for (Mesh* mesh : scene.first)
		if (mesh->isSelected()) selectedMesh = mesh;
	for (vector<Mesh*> mesh : scene.second) {
		for (Mesh* subMesh : mesh) {
			if (subMesh->isSelected()) {
				selectedMeshes = mesh;
				break;
			}
		}
	}
	for (Mesh* bubble : bubbles)
		if (bubble->isSelected()) selectedMesh = bubble;

	// Filtra il tasto premuto
	switch (key) {
		case 27:	// ESC
			glutLeaveMainLoop();
			break;
		case 'w':	// Avanti
			moveCameraForward();
			break;
		case 'a':	// Sinistra
			moveCameraLeft();
			break;
		case 's':	// Indietro
			moveCameraBackward();
			break;
		case 'd':	// Destra
			moveCameraRight();
			break;
		case 'e':	// In alto
			moveCameraUpward();
			break;
		case 'q':	// In basso
			moveCameraDownward();
			break;
		case 'x':	// Asse X
			currentAxis = X;
			workingAxis = "X";
			break;
		case 'y':	// Asse Y
			currentAxis = Y;
			workingAxis = "Y";
			break;
		case 'z':	// Asse Z
			currentAxis = Z;
			workingAxis = "Z";
			break;
		case 'T':	// Traslazione
			currentTransformation = TRANSLATION;
			transformation = "Translation";
			break;
		case 'R':	// Rozione
			currentTransformation = ROTATION;
			transformation = "Rotation";
			break;
		case 'S':	// Scalatura
			currentTransformation = SCALE;
			transformation = "Scale";
			break;
		case '+':	// Incrementa
			if (selectedMesh != NULL)
				applyTransformation(selectedMesh, 0.05f);
			else if (selectedMeshes.size() != 0) {
				for (Mesh* mesh : selectedMeshes)
					applyTransformation(mesh, 0.05f);
			}
			break;
		case '-':	// Decrementa
			if (selectedMesh != NULL)
				applyTransformation(selectedMesh, -0.05f);
			else if (selectedMeshes.size() != 0) {
				for (Mesh* mesh : selectedMeshes)
					applyTransformation(mesh, -0.05f);
			}
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void zoom(int button, int direction, int x, int y) {
	if (direction > 0) cameraPerspective.fov -= 2.0f;
	else cameraPerspective.fov += 2.0f;

	// Limita la prospettiva se lo zoom è troppo lontano o troppo vicino
	if (cameraPerspective.fov < 1.0f) cameraPerspective.fov = 1.0f;
	if (cameraPerspective.fov > 180.0f) cameraPerspective.fov = 180.0f;

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (state != GLUT_DOWN) return;

	// Filtra il click
	switch (button) {
		case GLUT_LEFT_BUTTON:
		{
			// Deseleziona la mesh precedentemente selezionata
			for (Mesh* mesh : scene.first)
				if (mesh->isSelected()) mesh->toggleSelection();
			for (vector<Mesh*> mesh : scene.second) {
				for (Mesh* subMesh : mesh)
					if (subMesh->isSelected()) subMesh->toggleSelection();
			}
			for (Mesh* bubble : bubbles)
				if (bubble->isSelected()) bubble->toggleSelection();

			vec3 ray = getRay(x, y);
			double closestIntersection = 0.0f;

			Mesh* previousIntersectedMesh = NULL;
			// Controlla se il click interseca una mesh, se ne trova una la seleziona e deseleziona quella precedente
			for (Mesh* mesh : scene.first) {
				double intersectionDistance = getIntersectionDistance(camera.position, ray, mesh);
				if (intersectionDistance > 0.0f && (intersectionDistance < closestIntersection || previousIntersectedMesh == NULL)) {
					if (previousIntersectedMesh != NULL)
						previousIntersectedMesh->toggleSelection();
					mesh->toggleSelection();
					previousIntersectedMesh = mesh;
					closestIntersection = intersectionDistance;
				}
			}
			for (vector<Mesh*> mesh : scene.second) {
				for (Mesh* subMesh : mesh) {
					double intersectionDistance = getIntersectionDistance(camera.position, ray, subMesh);
					if (intersectionDistance > 0.0f && (intersectionDistance < closestIntersection || previousIntersectedMesh == NULL)) {
						if (previousIntersectedMesh != NULL)
							previousIntersectedMesh->toggleSelection();
						subMesh->toggleSelection();
						previousIntersectedMesh = subMesh;
						closestIntersection = intersectionDistance;
					}
				}
			}
			for (Mesh* bubble : bubbles) {
				double intersectionDistance = getIntersectionDistance(camera.position, ray, bubble);
				if (intersectionDistance > 0.0f && (intersectionDistance < closestIntersection || previousIntersectedMesh == NULL)) {
					if (previousIntersectedMesh != NULL)
						previousIntersectedMesh->toggleSelection();
					bubble->toggleSelection();
					previousIntersectedMesh = bubble;
					closestIntersection = intersectionDistance;
				}
			}
			break;
		}
		default:
			break;
	}
	glutPostRedisplay();
}

vec3 getRay(int x, int y) {
	// Normalized Device Coordinates
	vec4 ndc = vec4(2.0f * x / WIDTH - 1.0f, 1.0f - 2.0f * y / HEIGHT, -1.0f, 1.0f);
	// Coordinate di clip
	vec4 clipCoordinates = vec4(ndc.x, ndc.y, ndc.z, 1.0f);
	// Coordinate di vista
	vec4 viewCoordinates = inverse(projectionMatrix) * clipCoordinates;
	viewCoordinates.w = 1.0;
	// Coordinate nel mondo
	vec4 worldCoordinates = inverse(view) * viewCoordinates;
	// Direction del raggio che parte congiunge la telecamera con la posizione del cursore
	vec3 ray = normalize(vec3(worldCoordinates) - vec3(camera.position));

	return ray;
}

double getIntersectionDistance(vec3 origin, vec3 direction, Mesh* mesh) {
	vec3 sphereCenter = vec3(mesh->getAnchorWorld()->x, mesh->getAnchorWorld()->y, mesh->getAnchorWorld()->z);
	float b = dot(direction, (origin - sphereCenter));
	double q = dot((origin - sphereCenter), (origin - sphereCenter)) - pow(mesh->getSphereRadius(), 2);
	double delta = pow(b, 2) - q;
	double t0 = - b - sqrt(delta);

	if (delta < 0.0f)
		return -1.0f;
	else
		return t0;
}

void modifyModelMatrix(Mesh* mesh, vec3 translationVector, vec3 rotationVector, GLfloat rotationAngle, GLfloat scaleValue) {
	mat4 traslationMatrix = translate(mat4(1), translationVector);
	mat4 scaleMatrix = scale(mat4(1), vec3(scaleValue, scaleValue, scaleValue));
	mat4 rotationMatrix = rotate(mat4(1), rotationAngle, rotationVector);

	mesh->setModel(*mesh->getModel() * scaleMatrix * rotationMatrix * traslationMatrix);
}

void applyTransformation(Mesh* mesh, float transformationValue) {
	vec3 axis;
	switch (currentAxis) {
		case X:
			axis = vec3(1.0f, 0.0f, 0.0f);
			break;
		case Y:
			axis = vec3(0.0f, 1.0f, 0.0f);
			break;
		case Z:
			axis = vec3(0.0f, 0.0f, 1.0f);
			break;
		default:
			return;
	}
	
	switch (currentTransformation) {
		case TRANSLATION:
			modifyModelMatrix(mesh, axis * transformationValue, axis, 0.0f, 1.0f);
			break;
		case ROTATION:
			modifyModelMatrix(mesh, vec3(0.0f), axis, transformationValue, 1.0f);
			break;
		case SCALE:
			modifyModelMatrix(mesh, vec3(0.0f), axis, 0.0f, 1.0f + transformationValue);
			break;
		default:
			return;
	}
}