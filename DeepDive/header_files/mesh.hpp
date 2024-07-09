#pragma once
#include "../lib.h"
#include "geometry.h"
#include "light_materials.hpp"

class Mesh {

	private:
		string name;
		vector<Vertex> vertices;
		vector <GLuint> indices;
		vector<vec3> normals;
		vector<vec2> textureCoordinates;
		mat4 model;
		GLuint VAO;
		GLuint VBO_V;
		GLuint VBO_C;
		GLuint VBO_N;
		GLuint VBO_T;
		GLuint EBO;
		Shader shader;
		MaterialType materialType;
		Material material;
		vec4 anchorObj;
		vec4 anchorWorld;
		float sphereRadius;
		bool selected;
		bool useTexture;
		unsigned int textureId;

	public:
		Mesh();
		string getName();
		void setName(string name);
		vector<Vertex>* getVertices();
		void setVertices(vector<Vertex> vertices);
		vector<vec3> getVerticesCoordinates();
		vector<vec4> getVerticesColors();
		vector<GLuint>* getIndices();
		void setIndices(vector<GLuint> indices);
		vector<vec3>* getNormals();
		void setNormals(vector<vec3> normals);
		vector<vec2>* getTextureCoordinates();
		void setTextureCoordinates(vector<vec2> textureCoordinates);
		mat4* getModel();
		void setModel(mat4 model);
		GLuint* getVAO();
		GLuint* getVerticesVBO();
		GLuint* getColorsVBO();
		GLuint* getNormalsVBO();
		GLuint* getTextureVBO();
		GLuint* getEBO();
		vec4* getAnchorObj();
		void setAnchorObj(vec4 anchor);
		vec4* getAnchorWorld();
		void setAnchorWorld(vec4 anchor);
		Shader getShader();
		void setShader(Shader shader);
		MaterialType getMaterialType();
		void setMaterialType(MaterialType materialType);
		Material getMaterial();
		void setMaterial(Material material);
		float getSphereRadius();
		void setSphereRadius(float radius);
		bool isSelected();
		void toggleSelection();
		bool isUseTextureActive();
		void toggleUseTexture();
		unsigned int getTexture();
		void setTexture(unsigned int textureId);

};