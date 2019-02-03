/*
Projekt PGPa + GMU
David Novák, xnovak1l
2018
*/
/*#pragma once

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

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
	glm::vec3 diffColor = glm::vec3(1.0f);
	float refractionIndex = 1.0f;
	glm::vec3 gap;
	float metalness = 0.0f;
} material;

class Scene {

public:

	Scene(){}
	~Scene();

	bool loadModel(std::string filename, std::string dir, bool loadMaterials = true, glm::mat4 transform = glm::mat4());

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

};*/

/*

*/

#pragma once

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <AssimpModelLoader.h>
#include <geSG/MeshTriangleIterators.h>
#include <geSG/MeshPrimitiveIterator.h>
#include <geSG/Model.h>
#include <geSG/Mesh.h>
#include <geSG/AttributeDescriptor.h>

#include <iostream>
#include <vector>

class Scene {

public:

	typedef struct {
		glm::vec4 coord_a, coord_b, coord_c;
		glm::vec4 normal_a, normal_b, normal_c;
		glm::vec2 uv_a, uv_b, uv_c;
		int material_id, align;
	} gpu_triangle;

	typedef struct {
		glm::vec3 diffuseColor;
		float refIndex;
		glm::vec3 gap;
		float align;
	} gpu_material;

	Scene() { init(); }
	~Scene();

	bool loadScene(std::string file);
	bool prepareGeometry(ge::sg::MeshIndexedTriangleIterator start, ge::sg::MeshIndexedTriangleIterator end);

	std::vector<gpu_triangle>& getGeometry();
	std::vector<gpu_material>& getMaterials();

private:

	void init();

	AssimpModelLoader ml;
	std::shared_ptr<ge::sg::Mesh> whole_scene = std::make_shared<ge::sg::Mesh>();
	std::shared_ptr<ge::sg::AttributeDescriptor> positions = std::make_shared<ge::sg::AttributeDescriptor>();
	std::shared_ptr<ge::sg::AttributeDescriptor> indices = std::make_shared<ge::sg::AttributeDescriptor>();

	std::vector<gpu_triangle> triangles;
	std::vector<gpu_material> materials;

};