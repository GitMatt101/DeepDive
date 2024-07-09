#include "../header_files/VAO_manager.h"

void initMeshVAO(Mesh* mesh) {
	glGenVertexArrays(1, mesh->getVAO());
	glBindVertexArray(*mesh->getVAO());
	glGenBuffers(1, mesh->getVerticesVBO());
	glBindBuffer(GL_ARRAY_BUFFER, *mesh->getVerticesVBO());
	glBufferData(GL_ARRAY_BUFFER, mesh->getVerticesCoordinates().size() * sizeof(vec3), mesh->getVerticesCoordinates().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, mesh->getColorsVBO());
	glBindBuffer(GL_ARRAY_BUFFER, *mesh->getColorsVBO());
	glBufferData(GL_ARRAY_BUFFER, mesh->getVerticesColors().size() * sizeof(vec4), mesh->getVerticesColors().data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, mesh->getNormalsVBO());
	glBindBuffer(GL_ARRAY_BUFFER, *mesh->getNormalsVBO());
	glBufferData(GL_ARRAY_BUFFER, mesh->getNormals()->size() * sizeof(vec3), mesh->getNormals()->data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	if (mesh->isUseTextureActive()) {
		glGenBuffers(1, mesh->getTextureVBO());
		glBindBuffer(GL_ARRAY_BUFFER, *mesh->getTextureVBO());
		glBufferData(GL_ARRAY_BUFFER, mesh->getTextureCoordinates()->size() * sizeof(vec2), mesh->getTextureCoordinates()->data(), GL_STATIC_DRAW);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(3);
	}

	glGenBuffers(1, mesh->getEBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *mesh->getEBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndices()->size() * sizeof(GLuint), mesh->getIndices()->data(), GL_STATIC_DRAW);
}

void initTextVAO(GLuint* textVAO, GLuint* textVBO) {
	glGenVertexArrays(1, textVAO);
	glGenBuffers(1, textVBO);
	glBindVertexArray(*textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, *textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}