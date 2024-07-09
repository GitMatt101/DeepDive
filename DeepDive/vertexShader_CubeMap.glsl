#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 Projection;
uniform mat4 View;
out vec3 TexCoords;

void main() {
     TexCoords = aPos;//vec3(aPos.x,-aPos.yz) ;
     mat4 Static_View=mat4(mat3(View));
     gl_Position = Projection * Static_View * vec4(aPos, 1.0);
}