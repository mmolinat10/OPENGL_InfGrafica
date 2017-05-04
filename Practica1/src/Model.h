#pragma once
#include <GL\glew.h>
#include "Shader.h"
#include <vector>
#include <string>
#include "Mesh.h"
#include <SOIL.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Model
{
public:
	Model::Model();
	Model::Model(GLchar* path);
	void Model::Draw(Shader shader, GLint drawMode);
private:
	std::vector<Mesh> meshes;
	std::string directory;
	void Model::loadModel(std::string path);
	void Model::processNode(aiNode* node, const aiScene* scene);
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLint Model::TextureFromFile(const char* path, std::string directory);
};

