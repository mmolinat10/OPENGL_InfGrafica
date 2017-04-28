//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include <SOIL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


using namespace std;
using namespace glm;


const GLint WIDTH = 800, HEIGHT = 600;
const int floatsPerPosition = 3;
bool WIREFRAME = false;
const int numIndices = 6;
const int sizeOfIndices = sizeof(int) * numIndices;
GLuint textures[2];
float mixOp;
bool fade = false;
GLint locTex1;
GLint locTex2;
GLint mixID;
GLint matrixPlaneID;
GLint projID;
uint directionRotate;
vec3 cameraPos;


// Positions of vertices on CPU
GLfloat VertexBufferObject[] = {
	//geometry				//colors			//texture coords
	  0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f, //Top right
	  0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, //Bottom right
	 -0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, //Bottom left
	 -0.5f, 0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f	//Top left
};

// Indexes on CPU
//(EBO)
int indices[] = {
	2, 0, 3,
	1, 0, 2 };


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cuando se pulsa una tecla escape cerramos la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//modo wireframe
	if (key == GLFW_KEY_W && action == GLFW_PRESS && WIREFRAME == false) {
		WIREFRAME = true;
	}

	//no wireframe
	else if (key == GLFW_KEY_W && action == GLFW_PRESS && WIREFRAME == true) {
		WIREFRAME = false;
	}

	if (key == GLFW_KEY_UP&&action == GLFW_PRESS) {
		fade = true;
	}
	else if (key == GLFW_KEY_DOWN&&action == GLFW_PRESS) {
		fade = false;
	}

	if (key == GLFW_KEY_LEFT&&action == GLFW_PRESS) {
		directionRotate = 1;
	}
	else if (key == GLFW_KEY_RIGHT&&action == GLFW_PRESS) {
		directionRotate = 2;
	}
}

int main() {
	mixOp = 0.0f;

	glfwSetErrorCallback(error_callback);

	//initGLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//set GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//create a window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica1_MarcMolina", NULL, NULL);
	if (!window)
	{
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//set GLEW and inicializate
	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}

	int screenWithd, screenHeight;
	glfwGetFramebufferSize(window, &screenWithd, &screenHeight);

	//que funcion se llama cuando se detecta una pulsaci�n de tecla en la ventana x
	glfwSetKeyCallback(window, key_callback);

	Shader shad = Shader("./src/textureVertex3d.vertexshader", "./src/textureFragment3d.fragmentshader");
	GLuint vao, vbo, ebo;
	
	//cargamos los shader

	//practica wireframe
	glGenVertexArrays(1, &vao); // Create new VAO
								
	glGenBuffers(1, &vbo); // Create new VBO
						   // Create new buffer that will be used to store indices
	glGenBuffers(1, &ebo);
			

	glBindVertexArray(vao); {// Binded VAO will store connections between VBOs and attributes
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind vbo as current vertex buffer
											// initialize vertex buffer, allocate memory, fill it with data
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);
		// Bind index buffer to corresponding target
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		// ititialize index buffer, allocate memory, fill it with data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);
		// indicate that current VBO should be used with vertex attribute with index 0
		glEnableVertexAttribArray(0);
		// indicate how vertex attribute 0 should interpret data in connected VBO
		glVertexAttribPointer(0, floatsPerPosition, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		/*color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, floatsPerPosition, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		*/
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}glBindVertexArray(0);
	
	int width, height, width2, height2;
	unsigned char* image1;
	unsigned char* image2;
	
	//texture
	glGenTextures(2, textures);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate the texture
	image1 = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);

	SOIL_free_image_data(image1);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate the texture
	image2 = SOIL_load_image("./src/img.png", &width2, &height2, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);

	SOIL_free_image_data(image2);

	glBindTexture(GL_TEXTURE_2D, 0);

	//glGenerateMipmap(GL_TEXTURE_2D);
	
	cameraPos = vec3(0.0f, 0.0f, -3.0f);
	
	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{	

		//origen de la camara, dimensiones de la ventana
		glViewport(0, 0, screenWithd, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT);

		//color de fondo
		glClearColor(1.0f, 1.0f, 0.8f, 1.0f);

		//definir la matriz de proyeccion
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-10, 10, -10.f, 10.f, -1.0f, 10.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		/*
		temporalmente desactivado a partir de la practica de transformaciones para ver la textura al escalar en Y negativa y Y positiva
		////activar el culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		////GL_CW sentido horario, GL_CCW sentido antihorario
		glFrontFace(GL_CCW);
		*/

		locTex1 = glGetUniformLocation(shad.Program, "texture1");
		locTex2 = glGetUniformLocation(shad.Program, "texture2");
		mixID = glGetUniformLocation(shad.Program, "mixOp");
		matrixPlaneID = glGetUniformLocation(shad.Program, "finalMatrix");

		//establecer el shader
		shad.USE();	

		glUniform1f(mixID, mixOp);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(locTex1, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glUniform1i(locTex2, 1);

		glBindVertexArray(vao);


		if (WIREFRAME) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}


		if (fade) {
			if (mixOp >= 0 && mixOp<1) {
				mixOp += 0.01f;
			}
		}
		else {
			if (mixOp>0.01f) {
				mixOp -= 0.01f;
			}
		}

		mat4 projCamera;
		projCamera = perspective(radians(70.f), (GLfloat)WIDTH / (GLfloat) HEIGHT, 0.1f, 100.0f);
		mat4 posCamera;
		posCamera = translate(posCamera, cameraPos);
		mat4 matrixPlane;
		
		//en opengl el orden es: primero trasladar, luego rotar y por último scalar
		
		/*matrix = translate(matrix, vec3(0.5f, 0.5f, 0));
		if (directionRotate == 1) {
			matrix = rotate(matrix, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if(directionRotate == 2) {
			matrix = rotate(matrix, (GLfloat)glfwGetTime() * glm::radians(-50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		}
	
		matrix = scale(matrix, vec3(0.5f, -0.5f, 0.0f));
		*/
		matrixPlane = translate(matrixPlane, vec3(0.0f, -0.5f, 0.0f));
		matrixPlane = rotate(matrixPlane, radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		matrixPlane = projCamera * posCamera * matrixPlane;
	
		
		glUniformMatrix4fv(matrixPlaneID, 1, GL_FALSE, value_ptr(matrixPlane));


		//pitar el VAO
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

		// reset bindings for VAO
		glBindVertexArray(0);

		//intercambia el framebuffer
		glfwSwapBuffers(window);
		//comprueba que algun disparador se halla activado (tales como el teclado, raton, etc)
		glfwPollEvents();
	}
	// // reset bindings for VAO, VBO and EBO and set free also with glDeleteVertexArrays()
	
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//glDeleteVertexArrays(1, &ebo);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

}

