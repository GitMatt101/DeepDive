#include "../lib.h"
#include "../header_files/camera.h"
#include "../header_files/view.h"

extern View camera;
extern int mouseX;
extern int mouseY;
extern float Theta;
extern float Phi;

vec4 getDirection();

void moveCameraForward() {
	vec4 direction = getDirection();
	camera.position += direction * CAMERA_SPEED;
}

void moveCameraBackward() {
	vec4 direction = getDirection();
	camera.position -= direction * CAMERA_SPEED;
}

void moveCameraRight() {
	vec4 direction = getDirection();
	vec3 slideVector = cross(vec3(direction), vec3(camera.upVector)) * CAMERA_SPEED;
	camera.position += vec4(slideVector, 0.0f);
	camera.target += vec4(slideVector, 0.0f);
}

void moveCameraLeft() {
	vec4 direction = getDirection();
	vec3 slideVector = cross(vec3(direction), vec3(camera.upVector)) * CAMERA_SPEED;
	camera.position -= vec4(slideVector, 0.0f);
	camera.target -= vec4(slideVector, 0.0f);
}

void moveCameraUpward() {
	camera.position += camera.upVector * CAMERA_SPEED;
	camera.target += camera.upVector * CAMERA_SPEED;
}

void moveCameraDownward() {
	camera.position -= camera.upVector * CAMERA_SPEED;
	camera.target -= camera.upVector * CAMERA_SPEED;
}

vec4 getDirection() {
	return camera.target - camera.position;
}

void passiveCameraMovement(int x, int y) {
	float alfa = 0.05;
	float centerX = (float)WIDTH / 2.0f;
	float centerY = (float)HEIGHT / 2.0f;

	float xoffset = x - centerX;
	float yoffset = (float)HEIGHT - y - centerY;
	mouseX = x;
	mouseY = y;

	xoffset *= alfa;
	yoffset *= alfa;
	Theta += xoffset;   //Aggiorno l'angolo Theta sommandogli l'offset della posizione x del mouse
	Phi += yoffset;  //Aggiorno l'angolo Phi sommandogli l'offset della posizione y del mouse 

	// Facciamo si' che l'angolo di Phi vari tra -90 e 90.
	if (Phi > 90.0f)
		Phi = 90.0f;
	if (Phi < -90.0f)
		Phi = -90.0f;

	//Ricavo le coordinate x,y,z del punto che sulla sfera è individuato dagli angoli Theta e Phi
	vec3 front;
	front.x = cos(radians(Theta)) * cos(radians(Phi));
	front.y = sin(radians(Phi));
	front.z = sin(radians(Theta)) * cos(radians(Phi));
	//Considero la direzione normalizzata (nota la quarta componente uguale a 0 indica una direzione
	camera.direction = vec4(normalize(front), 0.0); //Aggiorno la direzione della telecamera
	camera.target = camera.position + camera.direction; //aggiorno il punto in cui guarda la telecamera

	glutWarpPointer(centerX, centerY);
	glutPostRedisplay();
}
