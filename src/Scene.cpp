/*
Projekt PGPa + GMU
David Novák, xnovak1l
2018
*/
/*#define STB_IMAGE_IMPLEMENTATION
#include "Scene.h"

Scene::~Scene(){
	triangles.shrink_to_fit();
	materials.shrink_to_fit();
}

bool Scene::loadModel(std::string filename, std::string dir, bool loadMaterials, glm::mat4 transform){

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

	// Load vertices attributes
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

		vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));

		//if (mesh->mNormals != NULL)
			normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));

		if (mesh->mTextureCoords[0])
			uvs.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));

	}

	// Build triangles
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
	
		triangle t;

		aiFace face = mesh->mFaces[i];

		t.coord_a = glm::vec4(vertices[face.mIndices[0]], 1.0f);
		t.coord_b = glm::vec4(vertices[face.mIndices[1]], 1.0f);
		t.coord_c = glm::vec4(vertices[face.mIndices[2]], 1.0f);

		t.normal_a = glm::vec4(normals[face.mIndices[0]], 1.0f);
		t.normal_b = glm::vec4(normals[face.mIndices[1]], 1.0f);
		t.normal_c = glm::vec4(normals[face.mIndices[2]], 1.0f);

		if (uvs.size()) {
			t.uv_a = uvs[face.mIndices[0]];
			t.uv_b = uvs[face.mIndices[1]];
			t.uv_c = uvs[face.mIndices[2]];
		}
	
		t.material = mesh->mMaterialIndex - 1;
		
		triangles.push_back(t);
	}

	// Loading materials
	for (unsigned int i = 1; i < scene->mNumMaterials; i++) {

		material mat;

		// Diffuse color
		aiColor4D col;
		if (aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &col) == AI_SUCCESS) {
			mat.diffColor = glm::vec3(col.r, col.g, col.b);
		}

		// Refraction index
		if (aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_REFRACTI, &col) == AI_SUCCESS) {
			mat.refractionIndex = col.r;
		}

		if (aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_SPECULAR, &col) == AI_SUCCESS) {
			mat.metalness = col.r;
		}

		materials.push_back(mat);

	}

}*/

#include <Scene.h>

Scene::~Scene(){
	triangles.shrink_to_fit();
	materials.shrink_to_fit();
}

bool Scene::loadScene(std::string file){

	triangles.shrink_to_fit();
	materials.shrink_to_fit();
	printf("update\n");

	std::vector<float> tmp_pos, tmp_nor, tmp_uv;
	std::vector<unsigned> tmp_mat, tmp_ind;
	std::map<std::shared_ptr<ge::sg::Material>, int> asoc_mat;
	float* tmp;
	int mat_id = 0;
	int ind_offset = 0;

	/*gpu_triangle t;
	t.coord_a = glm::vec4(2.0f, 1.0f, 0.0f, 0.0f);
	t.coord_b = glm::vec4(4.0f, 1.0f, 0.0f, 0.0f);
	t.coord_c = glm::vec4(1.0f, 2.0f, 0.0f, 0.0f);
	triangles.push_back(t);*/

	auto scene = ml.loadScene(file.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (scene == nullptr)
		return false;

	printf("Loaded\n");

	for (auto model : scene->models) {

		for (auto material : model->materials) {

			asoc_mat.insert(std::pair<std::shared_ptr<ge::sg::Material>, int>(material, mat_id));
			mat_id++;

			gpu_material m;

			for (auto comp : material->materialComponents) {

				if (comp->getType() == ge::sg::MaterialComponent::ComponentType::SIMPLE) {
					ge::sg::MaterialSimpleComponent* ms = (ge::sg::MaterialSimpleComponent*)comp.get();
					if (ms->semantic == ge::sg::MaterialSimpleComponent::Semantic::diffuseColor) {

						float* col = (float*)ms->data.get();

						printf("color %f %f %f %f\n", col[0], col[1], col[2], col[3]);
						m.diffuseColor = glm::vec3(col[0], col[1], col[2]);
					}
				}

				if (comp->getType() == ge::sg::MaterialComponent::ComponentType::IMAGE) {
					ge::sg::MaterialImageComponent* mi = (ge::sg::MaterialImageComponent*)comp.get();
					if (mi->semantic == ge::sg::MaterialImageComponent::Semantic::diffuseTexture) {
						printf("texture %s\n", mi->filePath.c_str());
					}
				}
				
			}
			materials.push_back(m);
		}

		for (auto mesh : model->meshes) {

			//auto x = mesh->material;
			printf("mat %d\n", asoc_mat.at(mesh->material));

			tmp_mat.insert(tmp_mat.end(), mesh->count, asoc_mat.at(mesh->material));

			for (auto attr : mesh->attributes) {

				if (attr->semantic == ge::sg::AttributeDescriptor::Semantic::position) {
					tmp = static_cast<float*>(attr->data.get());
					std::copy(tmp, tmp + (attr->size / sizeof(float)), std::back_inserter(tmp_pos));
				}
				else if (attr->semantic == ge::sg::AttributeDescriptor::Semantic::normal) {
					tmp = static_cast<float*>(attr->data.get());
					std::copy(tmp, tmp + (attr->size / sizeof(float)), std::back_inserter(tmp_nor));
				}
				else if (attr->semantic == ge::sg::AttributeDescriptor::Semantic::texcoord) {
					tmp = static_cast<float*>(attr->data.get());
					std::copy(tmp, tmp + (attr->size / sizeof(float)), std::back_inserter(tmp_uv));
				}
				else if (attr->semantic == ge::sg::AttributeDescriptor::Semantic::indices) {
					//unsigned *ind = (unsigned*) (attr->data.get());
					unsigned* ind = static_cast<unsigned*>(attr->data.get());
					std::copy(ind, ind + (attr->size / sizeof(unsigned)), std::back_inserter(tmp_ind));
					
					for (int i = ind_offset; i < tmp_ind.size(); i++)
						tmp_ind[i] += ind_offset;

					
					printf("INDEX %d %d %d\n", tmp_ind[ind_offset], tmp_ind[ind_offset + 1], attr->size/sizeof(unsigned));
					ind_offset = tmp_ind.size();
				}

			}
		}
	}

	whole_scene->count = tmp_pos.size();
	positions->size = tmp_pos.size() * sizeof(float);
	indices->size = tmp_ind.size() * sizeof(unsigned);
	positions->data.reset();
	float* a = new float[tmp_pos.size()];
	std::memcpy(a, tmp_pos.data(), sizeof(float) * tmp_pos.size());
	positions->data = std::shared_ptr<void>((void*)a);
	/*whole_scene.count = tmp_pos.size();
	ge::sg::AttributeDescriptor ad;
	ad.semantic = ge::sg::AttributeDescriptor::Semantic::position;
	ad.numComponents = 3;
	ad.type = ge::sg::AttributeDescriptor::DataType::FLOAT;
	ad.size = tmp_pos.size() * sizeof(float);
	ad.data = std::shared_ptr<void>((void*)tmp_pos.data());
	std::shared_ptr<ge::sg::AttributeDescriptor> pos_desc = std::shared_ptr<ge::sg::AttributeDescriptor>(&ad);
	whole_scene.attributes.push_back(pos_desc);*/

	printf("matsize %d\n", tmp_mat.size());
	//for (int i = 0, j = 0; i < tmp_pos.size(); i += 9, j += 6) {
	for (int i = 0; i < tmp_ind.size(); i += 3) {
		gpu_triangle t;

		t.coord_a = glm::vec4(tmp_pos[3 * tmp_ind[i]], tmp_pos[(3 * tmp_ind[i]) + 1], tmp_pos[(3 * tmp_ind[i]) + 2], 1.0f);
		t.coord_b = glm::vec4(tmp_pos[3 * tmp_ind[i + 1]], tmp_pos[(3 * tmp_ind[i + 1]) + 1], tmp_pos[(3 * tmp_ind[i + 1]) + 2], 1.0f);
		t.coord_c = glm::vec4(tmp_pos[3 * tmp_ind[i + 2]], tmp_pos[(3 * tmp_ind[i + 2]) + 1], tmp_pos[(3 * tmp_ind[i + 2]) + 2], 1.0f);

		t.normal_a = glm::vec4(tmp_nor[3 * tmp_ind[i]], tmp_nor[(3 * tmp_ind[i]) + 1], tmp_nor[(3 * tmp_ind[i]) + 2], 1.0f);
		t.normal_b = glm::vec4(tmp_nor[3 * tmp_ind[i + 1]], tmp_nor[(3 * tmp_ind[i + 1]) + 1], tmp_nor[(3 * tmp_ind[i + 1]) + 2], 1.0f);
		t.normal_c = glm::vec4(tmp_nor[3 * tmp_ind[i + 2]], tmp_nor[(3 * tmp_ind[i + 2]) + 1], tmp_nor[(3 * tmp_ind[i + 2]) + 2], 1.0f);

		if (!tmp_uv.empty()) {
			t.uv_a = glm::vec2(tmp_uv[2 * tmp_ind[i]], tmp_uv[(2 * tmp_ind[i]) + 1]);
			t.uv_b = glm::vec2(tmp_uv[2 * tmp_ind[i + 1]], tmp_uv[(2 * tmp_ind[i + 1]) + 1]);
			t.uv_c = glm::vec2(tmp_uv[2 * tmp_ind[i + 2]], tmp_uv[(2 * tmp_ind[i + 2]) + 1]);
		}

		t.material_id = tmp_mat[tmp_ind[i]];

		/*t.coord_a = glm::vec4(tmp_pos[i], tmp_pos[i + 1], tmp_pos[i + 2], 1.0f);
		t.coord_b = glm::vec4(tmp_pos[i + 3], tmp_pos[i + 4], tmp_pos[i + 5], 1.0f);
		t.coord_c = glm::vec4(tmp_pos[i + 6], tmp_pos[i + 7], tmp_pos[i + 8], 1.0f);

		t.normal_a = glm::vec4(tmp_nor[i], tmp_nor[i + 1], tmp_nor[i + 2], 1.0f);
		t.normal_b = glm::vec4(tmp_nor[i + 3], tmp_nor[i + 4], tmp_nor[i + 5], 1.0f);
		t.normal_c = glm::vec4(tmp_nor[i + 6], tmp_nor[i + 7], tmp_nor[i + 8], 1.0f);

		if (!tmp_uv.empty()) {
			t.uv_a = glm::vec2(tmp_uv[j], tmp_uv[j + 1]);
			t.uv_b = glm::vec2(tmp_uv[j + 2], tmp_uv[j + 3]);
			t.uv_c = glm::vec2(tmp_uv[j + 4], tmp_uv[j + 5]);
		}

		t.material_id = tmp_mat[i];*/

		triangles.push_back(t);
	}

	printf("vtx %f %d\n", tmp_pos[0], tmp_pos.size());

	return true;
}

bool Scene::prepareGeometry(ge::sg::MeshIndexedTriangleIterator start, ge::sg::MeshIndexedTriangleIterator end){



	return false;
}

std::vector<Scene::gpu_triangle>& Scene::getGeometry(){
	return triangles;
}

std::vector<Scene::gpu_material>& Scene::getMaterials(){
	return materials;
}

void Scene::init(){

	whole_scene->attributes.push_back(positions);
	whole_scene->attributes.push_back(indices);

	positions->semantic = ge::sg::AttributeDescriptor::Semantic::position;
	positions->numComponents = 3;
	positions->type = ge::sg::AttributeDescriptor::DataType::FLOAT;

	indices->semantic = ge::sg::AttributeDescriptor::Semantic::indices;
	indices->numComponents = 1;
	indices->type = ge::sg::AttributeDescriptor::DataType::UNSIGNED_INT;
	//ad.size = tmp_pos.size() * sizeof(float);
	//ad.data = std::shared_ptr<void>((void*)tmp_pos.data());
	//std::shared_ptr<ge::sg::AttributeDescriptor> pos_desc = std::shared_ptr<ge::sg::AttributeDescriptor>(&ad);

}
