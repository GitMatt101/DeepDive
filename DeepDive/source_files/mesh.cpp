#include "../header_files/mesh.hpp"

Mesh::Mesh() {
	this->name = "";
	this->vertices = vector<Vertex>();
	this->indices = vector<GLuint>();
	this->normals = vector<vec3>();
	this->textureCoordinates = vector<vec2>();
	this->model = mat4(1.0f);
	this->VAO = 0;
	this->VBO_V = 0;
	this->VBO_C = 0;
	this->VBO_N = 0;
	this->VBO_T = 0;
	this->EBO = 0;
	this->shader = {"NONE", 0};
	this->materialType = MaterialType::NO_MATERIAL;
	this->material = Material();
	this->anchorObj = vec4(1.0f);
	this->anchorWorld = vec4(0.0f);
	this->sphereRadius = 2.0f;
	this->selected = false;
	this->useTexture = false;
}

string Mesh::getName() {
	return this->name;
}

void Mesh::setName(string name) {
	this->name = name;
}

vector<Vertex>* Mesh::getVertices() {
	return &(this->vertices);
}

void Mesh::setVertices(vector<Vertex> vertices) {
	this->vertices = vertices;
}

vector<vec3> Mesh::getVerticesCoordinates() {
	vector<vec3> coordinates;
	for (Vertex vertex : this->vertices)
		coordinates.push_back(vertex.coordinates);
	return coordinates;
}

vector<vec4> Mesh::getVerticesColors() {
	vector<vec4> colors;
	for (Vertex vertex : this->vertices)
		colors.push_back(vertex.color);
	return colors;
}

vector<GLuint>* Mesh::getIndices() {
	return &(this->indices);
}

void Mesh::setIndices(vector<GLuint> indices) {
	this->indices = indices;
}

vector<vec3>* Mesh::getNormals() {
	return &(this->normals);
}

void Mesh::setNormals(vector<vec3> normals) {
	this->normals = normals;
}

vector<vec2>* Mesh::getTextureCoordinates() {
	return &(this->textureCoordinates);
}

void Mesh::setTextureCoordinates(vector<vec2> textureCoordinates) {
	this->textureCoordinates = textureCoordinates;
}

mat4* Mesh::getModel() {
	return &(this->model);
}

void Mesh::setModel(mat4 model) {
	this->model = model;
}

GLuint* Mesh::getVAO() {
	return &(this->VAO);
}

GLuint* Mesh::getVerticesVBO() {
	return &(this->VBO_V);
}

GLuint* Mesh::getColorsVBO() {
	return &(this->VBO_C);
}

GLuint* Mesh::getNormalsVBO() {
	return &(this->VBO_N);
}

GLuint* Mesh::getTextureVBO() {
	return &(this->VBO_T);
}

GLuint* Mesh::getEBO() {
	return &(this->EBO);
}

vec4* Mesh::getAnchorObj() {
	return &(this->anchorObj);
}

void Mesh::setAnchorObj(vec4 anchor) {
	this->anchorObj = anchor;
}

vec4* Mesh::getAnchorWorld() {
	return &(this->anchorWorld);
}

void Mesh::setAnchorWorld(vec4 anchor) {
	this->anchorWorld = anchor;
}

MaterialType Mesh::getMaterialType() {
	return this->materialType;
}

void Mesh::setMaterialType(MaterialType materialType) {
	this->materialType = materialType;
}

Material Mesh::getMaterial() {
	return this->material;
}

void Mesh::setMaterial(Material material) {
	this->material = material;
}

float Mesh::getSphereRadius() {
	return this->sphereRadius;
}

bool Mesh::isSelected() {
	return this->selected;
}

void Mesh::toggleSelection() {
	this->selected = !this->selected;
}

bool Mesh::isUseTextureActive() {
	return this->useTexture;
}

void Mesh::toggleUseTexture() {
	this->useTexture = !this->useTexture;
}

void Mesh::setSphereRadius(float radius) {
	this->sphereRadius = radius;
}

Shader Mesh::getShader() {
	return this->shader;
}

void Mesh::setShader(Shader shader) {
	this->shader = shader;
}

unsigned int Mesh::getTexture() {
	return this->textureId;
}

void Mesh::setTexture(unsigned int textureId) {
	this->textureId = textureId;
}