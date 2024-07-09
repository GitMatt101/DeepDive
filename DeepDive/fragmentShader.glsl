#version 330 core
in vec4 ourColor;
out vec4 FragColor;  

in vec3 N, L, R, V;
in vec2 frag_coord_st;

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

uniform sampler2D tex;
uniform int shader;

float strength = 0.1f;

void main()
{
    switch(shader) {
        case 0: case 3:
            FragColor = ourColor;
            break;
        case 1: case 4:
            FragColor = mix(ourColor, texture(tex, frag_coord_st), 0.3f);
            break;
        case 2:
            vec3 ambient = strength * light.power * material.ambient;
            float cosTheta = max(dot(L, N), 0);
            vec3 diffuse = light.power * light.color * cosTheta * material.diffuse;
            float cosAlpha =  pow(max(dot(V, R), 0), material.shininess);
            vec3 specular =  light.power * light.color * cosAlpha * material.specular;
            FragColor = vec4(ambient + diffuse + specular, 1.0f);
            FragColor = mix(FragColor, texture(tex, frag_coord_st), 0.3f);
            break;
        default:
            break;
    }
}