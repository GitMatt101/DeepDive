#include "../lib.h"
#include "../header_files/interactions.h"
#include "../header_files/mesh.hpp"
#include "../header_files/view.h"
#include "../header_files/camera.h"
#include <iostream>
using namespace std;

#define TRACK_BALL_SPEED 50.0f

extern pair<vector<Mesh*>, vector<vector<Mesh*>>> scene;
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

vec3 getRay(int x, int y);

double getIntersectionDistance(vec3 origin, vec3 direction, Mesh* mesh);

void modifyModelMatrix(Mesh* mesh, vec3 translationVector, vec3 rotationVector, GLfloat rotationAngle, GLfloat scaleValue);

void applyTransformation(Mesh* mesh, float transformationValue);

void keyboardEvent(unsigned char key, int x, int y) {
	Mesh* selectedMesh = NULL;
	for (Mesh* mesh : scene.first)
		if (mesh->isSelected()) selectedMesh = mesh;

	switch (key) {
		case 27:
			glutLeaveMainLoop();
			break;
		case 'w':	// Forward
			moveCameraForward();
			break;
		case 'a':	// Left
			moveCameraLeft();
			break;
		case 's':	// Backward
			moveCameraBackward();
			break;
		case 'd':	// Right
			moveCameraRight();
			break;
		case 'e':	// Upward
			moveCameraUpward();
			break;
		case 'q':	// Downward
			moveCameraDownward();
			break;
		case 'x':	// X axis
			currentAxis = X;
			workingAxis = "X";
			break;
		case 'y':	// Y axis
			currentAxis = Y;
			workingAxis = "Y";
			break;
		case 'z':	// Z axis
			currentAxis = Z;
			workingAxis = "Z";
			break;
		case 'T':	// Translation
			currentTransformation = TRANSLATION;
			transformation = "Translation";
			break;
		case 'R':	// Rotation
			currentTransformation = ROTATION;
			transformation = "Rotation";
			break;
		case 'S':	// Scale
			currentTransformation = SCALE;
			transformation = "Scale";
			break;
		case '+':	// Increase
			if (selectedMesh != NULL)
				applyTransformation(selectedMesh, 0.05f);
			break;
		case '-':	// Decrease
			if (selectedMesh != NULL)
				applyTransformation(selectedMesh, -0.05f);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void zoom(int button, int direction, int x, int y) {
	if (direction > 0) cameraPerspective.fov -= 2.0f;
	else cameraPerspective.fov += 2.0f;

	// Limits the perspective if it goes too far or too close
	if (cameraPerspective.fov < 1.0f) cameraPerspective.fov = 1.0f;
	if (cameraPerspective.fov > 180.0f) cameraPerspective.fov = 180.0f;

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (state != GLUT_DOWN) return;

	switch (button) {
		case GLUT_LEFT_BUTTON:
			vec3 ray = getRay(x, y);
			double closestIntersection = 0.0f;
			Mesh* previousIntersectedMesh = NULL;
			for (Mesh* mesh : scene.first)
				if (mesh->isSelected()) mesh->toggleSelection();

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
			break;
	}
	glutPostRedisplay();
}

vec3 getRay(int x, int y) {
	// Normalized Device Coordinates
	vec4 ndc = vec4(2.0f * x / WIDTH - 1.0f, 1.0f - 2.0f * y / HEIGHT, -1.0f, 1.0f);
	// Clip coordinates
	vec4 clipCoordinates = vec4(ndc.x, ndc.y, ndc.z, 1.0f);
	// View coordinates
	vec4 viewCoordinates = inverse(projectionMatrix) * clipCoordinates;
	viewCoordinates.w = 1.0;
	// World coordinates
	vec4 worldCoordinates = inverse(view) * viewCoordinates;
	// Direction of the ray that starts from the camera and reaches the clicked point
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

vec3 getTrackBallPoint(float x, float y) {
	float xNDC = (2.0f * x - (float)WIDTH) / (float)WIDTH;
	float yNDC = ((float)HEIGHT - 2.0f * y) / (float)HEIGHT;

	float arg = 1 - pow(xNDC, 2) - pow(yNDC, 2);
	float zNDC = arg > 0 ? sqrt(arg) : 0;

	return normalize(vec3(xNDC, yNDC, zNDC));
}