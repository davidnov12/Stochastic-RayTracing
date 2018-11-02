#pragma once

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include <stb_image.h>

#include <iostream>
#include <vector>
#include <chrono>

typedef struct {
	glm::vec4 coord_a, coord_b, coord_c;
	glm::vec4 normal_a, normal_b, normal_c;
	glm::vec2 uv_a, uv_b, uv_c;
	unsigned material, gap;
} triangle;

typedef struct {
	glm::vec3 diffColor;
	float refractionIndex;
	GLuint64 diffuse;
	GLuint64 normal;
} material;

class Scene {

public:

	Scene(){}
	~Scene();

	bool loadModel(std::string& filename, std::string& dir, bool loadMaterials = true, glm::mat4 transform = glm::mat4());

	std::vector<triangle>& getGeometry();
	std::vector<material>& getMaterials();

private:

	
	void processNode(aiNode *node, const aiScene *scene, glm::mat4& transform);
	void processMesh(aiMesh *mesh, const aiScene *scene, glm::mat4& transform);

	bool loadMaterials = false;
	std::string directory;
	std::vector<triangle> triangles;
	std::vector<material> materials;

	std::vector<unsigned int> texHandles;

};