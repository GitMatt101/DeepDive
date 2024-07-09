#pragma once
#include "../lib.h"

struct View {
	vec4 position;
	vec4 target;
	vec4 upVector;
	vec4 direction;
};

struct Perspective {
	float fov;			// Field of View
	float aspect;
	float nearPlane;
	float farPlane;
};