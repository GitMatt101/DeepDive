#version 330 core
layout (location = 0) in vec3 aPos;
layout (location =2 ) in vec3  aNormal;
 
uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

out vec3 normals;
out vec3 worldCoordinates;
 
void main() {
	normals = mat3(transpose(inverse(Model))) * aNormal;
	worldCoordinates = vec3(Model * vec4(aPos, 1.0f));
	gl_Position = Projection * View * Model * vec4(aPos, 1.0f);
} 