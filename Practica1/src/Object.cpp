#include "Object.h"

Object::Object(vec3 scal, vec3 rot, vec3 pos, FigureType typef) {

	GLfloat VertexBufferObject[] = {
		//front
		1.0f ,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		1.0f , -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		1.0f ,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,
		//back
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f , -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		1.0f ,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,

		1.0f ,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		//left
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		//right
		1.0f , -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f ,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		1.0f ,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

		1.0f ,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f , -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,
		//down
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		1.0f , -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		1.0f , -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,
		//up
		1.0f ,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		1.0f ,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		1.0f ,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &VAO); // Create new VAO

	glGenBuffers(1, &VBO); // Create new VBO

	glGenBuffers(1, &EBO); // Create new EBO

	glBindVertexArray(VAO); {// Binded VAO will store connections between VBOs and attributes
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind vbo as current vertex buffer
											// initialize vertex buffer, allocate memory, fill it with data
		switch (typef) {
		case Object::cube:
			glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), &VertexBufferObject[0], GL_STATIC_DRAW);

			//positions
			glEnableVertexAttribArray(0);
			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

			//normals
			glEnableVertexAttribArray(1);
			
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			
			break;
		default: 
			break;
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}glBindVertexArray(0);

	position = pos;
	rotation = rot;
	scale = scal;
	moveUp = moveDown = moveRight = moveLeft = false;
	rotateUp = rotateDown = rotateRight = rotateLeft = false;
	plusRot = 50.f;
}


Object::~Object() {}

void Object::Draw() {
	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
void Object::Move(vec3 translation) {
	position += translation;
}
void Object::Rotate(vec3 rota) {
	rotation += rota;
}
void Object::Scale(vec3 scal) {
	scale = scal;
}


void Object::MoveAndRot(float deltaTime) {
	if (moveUp) {
		Move(vec3(0.0f, 1.f * deltaTime, 0.0f));
	}
	if (moveDown) {
		Move(vec3(0.0f, -1.f * deltaTime, 0.0f));
	}
	if (moveLeft) {
		Move(vec3(-1.0f * deltaTime, 0.0f, 0.0f));
	}
	if (moveRight) {
		Move(vec3(1.0f * deltaTime, 0.0f, 0.0f));
	}

	if (rotateUp) {
		Rotate(vec3(-plusRot * deltaTime, 0.0f, 0.0f));
	}
	if (rotateDown) {
		Rotate(vec3(plusRot * deltaTime, 0.0f, 0.0f));
	}
	if (rotateLeft) {
		Rotate(vec3(0.0f, -plusRot * deltaTime, 0.0f));
	}
	if (rotateRight) {
		Rotate(vec3(0.0f, plusRot * deltaTime, 0.0f));
	}
	
}

void Object::DoMoveAndRot(GLFWwindow *window) {
	moveUp = glfwGetKey(window, GLFW_KEY_I);
	moveDown = glfwGetKey(window, GLFW_KEY_K);
	moveLeft = glfwGetKey(window, GLFW_KEY_J);
	moveRight = glfwGetKey(window, GLFW_KEY_L);
	rotateUp = glfwGetKey(window, GLFW_KEY_KP_8);
	rotateDown = glfwGetKey(window, GLFW_KEY_KP_2);
	rotateLeft = glfwGetKey(window, GLFW_KEY_KP_4);
	rotateRight = glfwGetKey(window, GLFW_KEY_KP_6);

}

mat4 Object::GetModelMatrix() {
	
	if (rotation.x >= 360)
		rotation.x = glm::mod(rotation.x, 360.f);
	if (rotation.x <= (-360))
		rotation.x = glm::mod(rotation.x, (-360.f));
	if (rotation.y >= 360)
		rotation.y = glm::mod(rotation.y, 360.f);
	if (rotation.y <= (-360))
		rotation.y = glm::mod(rotation.y, (-360.f));
	
	mat4 matrixModel;
	matrixModel = glm::translate(matrixModel, GetPosition());
	matrixModel = glm::rotate(matrixModel, glm::radians(rotation.x), vec3(1.0, 0.0, 0.0));
	matrixModel = glm::rotate(matrixModel, glm::radians(rotation.y), vec3(0.0, 1.0, 0.0));
	matrixModel = glm::scale(matrixModel, scale);
	return matrixModel;
}

vec3 Object::GetPosition() {
	return position;
}

void Object::Delete() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}