#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 coord_st;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;
uniform vec3 viewPos;
uniform int shader;
uniform float time;

struct LightPoint {
	vec3 position;
	vec3 color;
	float power;
};
uniform LightPoint light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

out vec4 ourColor;
out vec3 N, V, L, R;
out vec2 frag_coord_st;
float strenght = 0.1f;

void processLight() {
    vec4 eyePosition = View * Model * vec4(aPos, 1.0f);
    vec4 eyeLightPos = View * vec4(light.position, 1.0f);
    N = normalize(transpose(inverse(mat3(View * Model))) * vertexNormal);
    V = normalize(viewPos - eyePosition.xyz);
    L = normalize((eyeLightPos - eyePosition).xyz);
    R = reflect(-L, N);
}

void main()
{
    ourColor = aColor;
    vec4 v = vec4(aPos, 1.0f);

    switch(shader) {
        case 1: case 3:
            processLight();

            vec3 ambient = strenght * light.power * material.ambient;

            float cosTheta = max(dot(L, N), 0);
            vec3 diffuse = light.power * light.color * cosTheta * material.diffuse;

            float cosAlpha =  pow(max(dot(V, R), 0), material.shininess);
            vec3 specular =  light.power * light.color * cosAlpha * material.specular;

            ourColor = vec4(ambient + diffuse + specular, 1.0f);
            break;
        case 2:
            processLight();
            break;
        case 4:
            float offset = 0.3f * sin(0.5f * time + 5.0f * v.x);
            v.z = v.z + offset;
            break;
        default:
            break;
    }
    frag_coord_st = coord_st;
	gl_Position = Projection * View * Model * v;
}  
