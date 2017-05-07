#pragma once
#include <GL\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>
using namespace glm;
#include "Shader.h"
#include <GLFW\glfw3.h>

class Object {

public:
	enum FigureType {
		cube = 0,
	};

	Object::Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef);
	Object::~Object();

	void Object::Draw();
	void Object::Move(vec3 translation);
	void Object::Rotate(vec3 rota);
	void Object::Scale(vec3 scal);
	void Object::MoveAndRot(float deltaTime);
	void Object::DoMoveAndRot(GLFWwindow *window);
	void Object::Delete();
	mat4 Object::GetModelMatrix();
	vec3 Object::GetPosition();
	GLboolean moveUp, moveDown, moveLeft, moveRight;
	GLboolean rotateUp, rotateDown, rotateLeft, rotateRight;
	GLfloat plusRot;
	

private:
	GLuint VBO, VAO, EBO;
	vec3 position;
	vec3 scale;
	vec3 rotation;

};

