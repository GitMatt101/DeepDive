#include "../header_files/assimp.hpp"

void loadAssImp(const char* path, vector<Mesh*>* mesh, string name) {
	Assimp::Importer importer;

	const aiScene* aiScene = importer.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!aiScene) exit(-1);

	const aiMesh* aiMesh;

	for (unsigned int i = 0; i < aiScene->mNumMeshes; i++)
	{
		// Crea una nuova mesh e ne imposta il nome
		Mesh* subMesh = new Mesh();
		subMesh->setName(name);

		aiMesh = aiScene->mMeshes[i];
		aiMaterial* aiMaterial = aiScene->mMaterials[aiMesh->mMaterialIndex];

		aiColor3D color;
		float value;

		// Carica il materiale base della mesh e lo modifica in base al file .mtl
		Material material = subMesh->getMaterial();

		if (aiReturn_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color))
			material.ambient = vec3(color.r, color.g, color.b);

		if (aiReturn_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color))
			material.diffuse = vec3(color.r, color.g, color.b);
		else
			material.diffuse = vec3(1.0f, 0.2f, 0.1f);

		if (aiReturn_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color))
			material.specular = vec3(color.r, color.g, color.b);
		else
			material.specular = vec3(0.5f, 0.5f, 0.5f);

		if (aiReturn_SUCCESS == aiMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, value))
			material.shininess = value;
		else
			material.shininess = 50.0f;

		subMesh->setMaterial(material);

		// Carica i vertici, i vettori normali e le coordinate di texture
		vector<Vertex> vertices;
		vector<vec3> normals;
		vector<vec2> textureCoordinates;
		for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
			aiVector3D pos = aiMesh->mVertices[i];
			vertices.push_back({
				vec3(pos.x, pos.y, pos.z),
				vec4(0.0f, 0.0f, 0.0f, 1.0f)
			});

			float textureX = aiMesh->mTextureCoords[0][i].x;
			float textureY = aiMesh->mTextureCoords[0][i].y;
			textureCoordinates.push_back(vec2(textureX, textureY));
			aiVector3D n = aiMesh->mNormals[i];
			normals.push_back(vec3(n.x, n.y, n.z));
		}

		// Carica gli indici per la renderizzazione dei vertici
		vector<GLuint> indices;
		for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
			indices.push_back(aiMesh->mFaces[i].mIndices[0]);
			indices.push_back(aiMesh->mFaces[i].mIndices[1]);
			indices.push_back(aiMesh->mFaces[i].mIndices[2]);
		}

		// Inserisce tutti i valori finora caricati dentro alla mesh
		subMesh->setVertices(vertices);
		subMesh->setNormals(normals);
		subMesh->setIndices(indices);
		subMesh->setTextureCoordinates(textureCoordinates);

		// Carica la mesh dentro al vettore
		mesh->push_back(subMesh);
	}
}