#include "../header_files/geometry.h"

void createCuboid(float width, float height, float depth, vec4 color1, vec4 color2, vector<Vertex>* vertices, vector<GLuint>* indices) {
	float x = width / 2;
	float y = height / 2;
	float z = depth / 2;

	// Faccia frontale
	vertices->push_back(Vertex(vec3(-x, y, z), color1));
	vertices->push_back(Vertex(vec3(x, y, z), color2));
	vertices->push_back(Vertex(vec3(x, -y, z), color1));
	vertices->push_back(Vertex(vec3(-x, -y, z), color2));

	// Faccia Posteriore
	vertices->push_back(Vertex(vec3(-x, y, -z), color1));
	vertices->push_back(Vertex(vec3(x, y, -z), color2));
	vertices->push_back(Vertex(vec3(x, -y, -z), color1));
	vertices->push_back(Vertex(vec3(-x, -y, -z), color2));

	// Centro del parallelepipedo
	vertices->push_back(Vertex(vec3(0.0f, 0.0f, 0.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f)));

	// Indici
	indices->push_back(0); indices->push_back(1); indices->push_back(2);
	indices->push_back(2); indices->push_back(3); indices->push_back(0);
	indices->push_back(1); indices->push_back(5); indices->push_back(6);
	indices->push_back(6); indices->push_back(2); indices->push_back(1);
	indices->push_back(7); indices->push_back(6); indices->push_back(5);
	indices->push_back(5); indices->push_back(4); indices->push_back(7);
	indices->push_back(4); indices->push_back(0); indices->push_back(3);
	indices->push_back(3); indices->push_back(7); indices->push_back(4);
	indices->push_back(4); indices->push_back(5); indices->push_back(1);
	indices->push_back(1); indices->push_back(0); indices->push_back(4);
	indices->push_back(3); indices->push_back(2); indices->push_back(6);
	indices->push_back(6); indices->push_back(7); indices->push_back(3);
	indices->push_back(vertices->size() - 1);
}

void createEllipsoid(float rx, float ry, float rz, int precision, vec4 color, vector<Vertex>* vertices, vector<GLuint>* indices, vector<vec3>* normals, vector<vec2>* textureCoordinates) {
	vec3 center = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < precision; i++) {
		float step1 = i / (float)precision;
		float phi = step1 * PI;

		for (int j = 0; j < precision; j++) {
			float step2 = j / (float)precision;
			float theta = step2 * PI * 2;

			float x = center.x + rx * (cosf(theta) * sinf(phi));
			float y = center.y + ry * cosf(phi);
			float z = center.z + rz * sinf(theta) * sinf(phi);
			vertices->push_back(Vertex(vec3(x, y, z), color));
			normals->push_back(vec3(x, y, z));
			textureCoordinates->push_back(vec2(step2, step1));
		}
	}
	vertices->push_back(Vertex(center, color));

	for (int i = 0; i < precision * precision + precision; ++i) {
		indices->push_back(i);
		indices->push_back(i + precision + 1);
		indices->push_back(i + precision);
		indices->push_back(i + precision + 1);
		indices->push_back(i);
		indices->push_back(i + 1);
	}
	indices->push_back(vertices->size() - 1);
}

void createPanel(float width, float depth, float repeat, vec4 color, vector<Vertex>* vertices, vector<GLuint>* indices, vector<vec3>* normals, vector<vec2>* textureCoordinates) {
	float x = width / 2;
	float z = depth / 2;
	vertices->push_back({
		vec3(-x, 0.0f, z),
		color
	});
	vertices->push_back({
		vec3(x, 0.0f, z),
		color
	});
	vertices->push_back({
		vec3(x, 0.0f, -z),
		color
	});
	vertices->push_back({
		vec3(-x, 0.0f, -z),
		color
	});
	normals->push_back(vec3(0.0f, 1.0f, 0.0f));
	normals->push_back(vec3(0.0f, 1.0f, 0.0f));
	normals->push_back(vec3(0.0f, 1.0f, 0.0f));
	normals->push_back(vec3(0.0f, 1.0f, 0.0f));
	textureCoordinates->push_back(vec2(0.0f, repeat));
	textureCoordinates->push_back(vec2(repeat, repeat));
	textureCoordinates->push_back(vec2(repeat, 0.0f));
	textureCoordinates->push_back(vec2(0.0f, 0.0f));
	indices->push_back(0); indices->push_back(1); indices->push_back(2);
	indices->push_back(0); indices->push_back(2); indices->push_back(3);

	vertices->push_back({
		vec3(0.0f, 0.0f, 0.0f),
		color
	});
	indices->push_back(vertices->size() - 1);
}