#version 330 core

uniform vec3 viewPos;

in vec3 normals;
in vec3 worldCoordinates;

uniform samplerCube cubemap;
out vec4 FragColor;

void main() {
    vec3 E = normalize(worldCoordinates - viewPos);
    vec3 R = reflect(E, normalize(normals));
    FragColor =  vec4(texture(cubemap, R).rgb, 1.0f);
}