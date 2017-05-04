#pragma once
#include <GL\glew.h>
#include "Shader.h"
#include <vector>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <gtc\matrix_transform.hpp>
#include <assimp\types.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Mesh::Mesh(std::vector<Vertex> vert, std::vector<GLuint> ind, std::vector<Texture> text);
	void Mesh::Draw(Shader MeshShader, GLint DrawMode);
private:
	GLuint VAO, VBO, EBO;
	void Mesh::setupMesh();

};

