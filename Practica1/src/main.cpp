//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include <SOIL.h>

using namespace std;


const GLint WIDTH = 800, HEIGHT = 600;
const int floatsPerPosition = 3;
bool WIREFRAME = false;
const int numIndices = 6;
const int sizeOfIndices = sizeof(int) * numIndices;
GLuint texture;
int widthImage = 512, heightImage = 512;

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
}

int main() {

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

	Shader shad = Shader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");
	GLuint vao, vbo, ebo;
	
	//cargamos los shader

	//practica wireframe
	glGenVertexArrays(1, &vao); // Create new VAO
								// Binded VAO will store connections between VBOs and attributes
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo); // Create new VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind vbo as current vertex buffer
										 // initialize vertex buffer, allocate memory, fill it with data
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferObject), VertexBufferObject, GL_STATIC_DRAW);
	
	// indicate how vertex attribute 0 should interpret data in connected VBO
	glVertexAttribPointer(0, floatsPerPosition, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

	// indicate that current VBO should be used with vertex attribute with index 0
	glEnableVertexAttribArray(0);

	/*color
	glVertexAttribPointer(1, floatsPerPosition, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);*/

	//texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set the texture wrapping/filtering
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Load and generate the texture
	unsigned char* image = SOIL_load_image("./src/texture.png", &widthImage, &heightImage, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImage, heightImage, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	
	//end texture..

	// Create new buffer that will be used to store indices
	glGenBuffers(1, &ebo);
	// Bind index buffer to corresponding target
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// ititialize index buffer, allocate memory, fill it with data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	


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

		////activar el culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		////GL_CW sentido horario, GL_CCW sentido antihorario
		glFrontFace(GL_CCW);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		//establecer el shader
		shad.USE();

		//uniform variable
		//GLint variableShader = glGetUniformLocation(shad.Program, "offset");
		//glUniform1f(variableShader, abs(sin(glfwGetTime())) * 0.5f);
		GLint variableShader = glGetUniformLocation(shad.Program, "ourTexture");
		glUniform1i(variableShader, 0);

		glBindVertexArray(vao);

		if (WIREFRAME) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

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
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vbo);
	glDeleteVertexArrays(1, &ebo);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

}

