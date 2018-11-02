#define STB_IMAGE_IMPLEMENTATION
#include "Scene.h"

Scene::~Scene(){
	triangles.shrink_to_fit();
	materials.shrink_to_fit();
}

bool Scene::loadModel(std::string & filename, std::string & dir, bool loadMaterials, glm::mat4 transform){

	directory = dir;
	typedef std::chrono::high_resolution_clock Clock;
	auto t1 = Clock::now();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Scene loading failed" << std::endl; 
		return false;
	}

	processNode(scene->mRootNode, scene, transform);

	auto t2 = Clock::now();

	std::cout << "Scene loaded" << std::endl;
	std::cout << "Total " << triangles.size() << " triangles" << std::endl;
	std::cout << (t2 - t1).count() / 1000000000.0f << std::endl;

	return true;
}

std::vector<triangle>& Scene::getGeometry(){
	return triangles;
}

std::vector<material>& Scene::getMaterials(){
	return materials;
}

void Scene::processNode(aiNode * node, const aiScene * scene, glm::mat4& transform){

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene, transform);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, transform);
	}

}

void Scene::processMesh(aiMesh * mesh, const aiScene * scene, glm::mat4 & transform){

	std::vector<glm::vec3> vertices, normals;
	std::vector<glm::vec2> uvs;

	glm::mat3 invTr = glm::transpose(glm::inverse(glm::mat3(transform)));

	//std::cout << "properites: " << scene->mMaterials[0]->mNumProperties << std::endl;

	// Load vertices attributes
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

		vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

		if (mesh->mNormals != NULL)
			normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));

		if (mesh->mTextureCoords[0])
			uvs.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));

	}
	//std::cout << "Faces " << mesh->mNumFaces << std::endl;
	// Build triangles
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
	
		triangle t;

		aiFace face = mesh->mFaces[i];

		t.coord_a = glm::vec4(vertices[face.mIndices[0]], 1.0f);
		t.coord_b = glm::vec4(vertices[face.mIndices[1]], 1.0f);
		t.coord_c = glm::vec4(vertices[face.mIndices[2]], 1.0f);

		t.coord_a = t.coord_a * transform;
		t.coord_b = t.coord_b * transform;
		t.coord_c = t.coord_c * transform;

		t.normal_a = glm::vec4(invTr * normals[face.mIndices[0]], 1.0f);
		t.normal_b = glm::vec4(invTr * normals[face.mIndices[1]], 1.0f);
		t.normal_c = glm::vec4(invTr * normals[face.mIndices[2]], 1.0f);

		if (uvs.size()) {
			t.uv_a = uvs[face.mIndices[0]];
			t.uv_b = uvs[face.mIndices[1]];
			t.uv_c = uvs[face.mIndices[2]];
		}
	
		t.material = mesh->mMaterialIndex;

		triangles.push_back(t);
	}

	// Loading materials
	for (unsigned int i = 1; i < scene->mNumMaterials; i++) {

		material mat;

		std::cout << "Material " << i << std::endl;

		// Diffuse texture
		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;

			if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string absPath = directory + "/" + path.data;

				unsigned char* imageData;
				int width, height, comp;

				stbi_set_flip_vertically_on_load(true);
				imageData = stbi_load(absPath.c_str(), &width, &height, &comp, STBI_default);

				if (imageData == NULL){
					std::cerr << "Failed load diffuse texture " << absPath.c_str() << std::endl;
					break;
				}
				//printf("%d %d %d\n", imageData[0], imageData[1], imageData[2]);

				GLuint textureID;
				ge::gl::glGenTextures(1, &textureID);

				GLenum components = comp == 3 ? GL_RGB : comp == 1 ? GL_RED : GL_RGBA;
				
				ge::gl::glBindTexture(GL_TEXTURE_2D, textureID);

				ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, components, GL_UNSIGNED_BYTE, imageData);
				ge::gl::glGenerateMipmap(GL_TEXTURE_2D);
				
				ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				ge::gl::glBindTexture(GL_TEXTURE_2D, 0);
				texHandles.push_back(textureID);
				mat.diffuse = ge::gl::glGetTextureHandleARB(textureID);
				ge::gl::glMakeTextureHandleResidentARB(mat.diffuse);
				//printf("%d %d %d\n", imageData[0], imageData[1], imageData[2]);
				std::cout << "Diffuse texture loaded " << absPath.c_str() << std::endl;

				stbi_image_free(imageData);

			}

		}

		// Normal map
		if (scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS) > 0) {
			aiString path;

			if (scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string absPath = directory + "/" + path.data;

				unsigned char* imageData;
				int width, height, comp;

				stbi_set_flip_vertically_on_load(true);
				imageData = stbi_load(absPath.c_str(), &width, &height, &comp, STBI_default);

				if (imageData == NULL) {
					std::cerr << "Failed load normal map " << absPath.c_str() << std::endl;
					break;
				}
				
				GLuint textureID;
				ge::gl::glGenTextures(1, &textureID);

				GLenum components = comp == 3 ? GL_RGB : comp == 1 ? GL_RED : GL_RGBA;

				ge::gl::glBindTexture(GL_TEXTURE_2D, textureID);

				ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, components, GL_UNSIGNED_BYTE, imageData);
				ge::gl::glGenerateMipmap(GL_TEXTURE_2D);

				ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				ge::gl::glBindTexture(GL_TEXTURE_2D, 0);
				
				texHandles.push_back(textureID);
				mat.diffuse = ge::gl::glGetTextureHandleARB(texHandles.at(texHandles.size() - 1));
				ge::gl::glMakeTextureHandleResidentARB(mat.diffuse);

				std::cout << "Normal map loaded " << absPath.c_str() << std::endl;

				stbi_image_free(imageData);

			}

		}

		// Diffuse color
		aiColor4D col;
		if (aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &col) == AI_SUCCESS) {
			mat.diffColor = glm::vec3(col.r, col.g, col.b);

			std::cout << "Diffuse color loaded " << col.r << " " << col.g << " " << col.b << std::endl;
		}

		// Refraction index
		if (aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_REFRACTI, &col) == AI_SUCCESS) {
			mat.refractionIndex = col.r;

			std::cout << "Refraction index loaded " << col.r << std::endl;
		}

		materials.push_back(mat);

		std::cout << "-------------------------------------" << std::endl;
		//std::cout << scene->mMaterials[i]->

	}

}
