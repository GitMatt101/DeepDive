#include "../header_files/menu.hpp"
#include "../header_files/light_materials.hpp"
#include "../header_files/mesh.hpp"

extern vector<Material> materials;
extern vector<Shader> shaders;
extern pair<vector<Mesh*>, vector<vector<Mesh*>>> scene;

void materialMenu(int option);
void shaderMenu(int option);
void mainMenu(int option);

void buildOpenGLMenu() {
	int materialSubMenu = glutCreateMenu(materialMenu);

	glutAddMenuEntry(materials[MaterialType::EMERALD].name.c_str(), MaterialType::EMERALD);
	glutAddMenuEntry(materials[MaterialType::BRASS].name.c_str(), MaterialType::BRASS);
	glutAddMenuEntry(materials[MaterialType::SLATE].name.c_str(), MaterialType::SLATE);
	glutAddMenuEntry(materials[MaterialType::YELLOW].name.c_str(), MaterialType::YELLOW);

	int shaderSubMenu = glutCreateMenu(shaderMenu);
	glutAddMenuEntry(shaders[ShaderOption::NONE].name.c_str(), ShaderOption::NONE);
	glutAddMenuEntry(shaders[ShaderOption::GOURAD_SHADING].name.c_str(), ShaderOption::GOURAD_SHADING);
	glutAddMenuEntry(shaders[ShaderOption::PHONG_SHADING].name.c_str(), ShaderOption::PHONG_SHADING);
	glutAddMenuEntry(shaders[ShaderOption::NO_TEXTURE].name.c_str(), ShaderOption::NO_TEXTURE);
	glutAddMenuEntry(shaders[ShaderOption::WAVE].name.c_str(), ShaderOption::WAVE);

	glutCreateMenu(mainMenu);
	glutAddMenuEntry("Opzioni", -1);
	glutAddMenuEntry("", -1);
	glutAddMenuEntry("Wireframe", MenuOption::WIRE_FRAME);
	glutAddMenuEntry("Face fill", MenuOption::FACE_FILL);
	glutAddSubMenu("Material", materialSubMenu);
	glutAddSubMenu("Shader", shaderSubMenu);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void materialMenu(int option) {
	for (Mesh* mesh : scene.first) {
		if (mesh->isSelected())
			mesh->setMaterialType((MaterialType)option);
	}
	glutPostRedisplay();
}

void shaderMenu(int option) {
	for (Mesh* mesh : scene.first) {
		if (mesh->isSelected())
			mesh->setShader(shaders[option]);
	}
	glutPostRedisplay();
}

void mainMenu(int option) {
	switch (option) {
		case MenuOption::WIRE_FRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case MenuOption::FACE_FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}