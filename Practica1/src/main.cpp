﻿//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include <SOIL.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


using namespace std;
using namespace glm;

GLFWwindow* window;
const GLuint WIDTH = 800, HEIGHT = 600;
//const int floatsPerPosition = 3;
bool WIREFRAME = false;
const int numIndices = 6;
const int sizeOfIndices = sizeof(int) * numIndices;
GLuint textures[2];
float mixOp;
bool fade = false;
GLint locTex1;
GLint locTex2;
GLint mixID;
//GLint matrixPlaneID;
//uint directionRotate;
float plusRot;

bool plusRotRight, plusRotLeft, plusRotUp, plusRotDown;
float rotX, rotY = 0.0f;
//camara
Camera cam;
/*bool moveForward, moveBackwards, moveRight, moveLeft;*/
double mouseLastPosX, mouseLastPosY;
bool start;
float _pitch, _yaw;
mat4 myLook;
mat4 mLook;
vec3 cameraPos;
vec3 cameraDir;
vec3 cameraRight;
vec3 cameraUp;
float sensibility;
float FOV = 60.0f;
float cameraSpeed;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/*void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void DoMovement(GLFWwindow* window);*/
//mat4 miLookAt(vec3 position, vec3 target, vec3 worldUp);
void PrintAndCompareMatrix(mat4 m1, mat4 m2);
void PrintMatrix(mat4 m);
void cursor_callback(GLFWwindow* window, double xPos, double yPos);

/*
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
*/

GLfloat VertexBufferCube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

vec3 CubesPositionBuffer[] = {
	vec3(0.0f ,  0.0f,  0.0f),
	vec3(2.0f ,  5.0f, -15.0f),
	vec3(-1.5f, -2.2f, -2.5f),
	vec3(-3.8f, -2.0f, -12.3f),
	vec3(2.4f , -0.4f, -3.5f),
	vec3(-1.7f,  3.0f, -7.5f),
	vec3(1.3f , -2.0f, -2.5f),
	vec3(1.5f ,  2.0f, -2.5f),
	vec3(1.5f ,  0.2f, -1.5f),
	vec3(-1.3f,  1.0f, -1.5f)
};

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void mousePosWrapper(GLFWwindow *windowP, double xpos, double ypos)
{
	windowP = window;
	if (&cam)
	{
		cam.MouseMove(windowP, xpos, ypos);
	}
}

void mouseScrollWrapper(GLFWwindow *windowP, double xScroll, double yScroll)
{
	windowP = window;
	if (&cam)
	{
		cam.MouseScroll(windowP, xScroll, yScroll);
	}
}

/*
mat4 miLookAt(vec3 position, vec3 target, vec3 worldUp) {
	
	vec3 zaxis = normalize(position - target);
	
	vec3 xaxis = normalize(cross(normalize(worldUp), zaxis));
	
	vec3 yaxis = cross(zaxis, xaxis);

	mat4 translation, rotation; 
	translation[3][0] = -position.x; 
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;
	translation[3][3] = 1;

	rotation[0][0] = xaxis.x; 
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x; 
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x; 
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;
	rotation[3][3] = 1;

	return rotation * translation;
}*/

void PrintMatrix(mat4 m) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "m[" << i << "][" << j << "] = " << m[i][j] << endl;

		}
	}
}

void PrintAndCompareMatrix(mat4 m1, mat4 m2) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cout << "m1[" << i << "][" << j << "] = " << m1[i][j] << endl;
			cout << "m2[" << i << "][" << j << "] = " << m2[i][j] << endl;
		}
	}
}


void cursor_callback(GLFWwindow* window, double xPos, double yPos) {
	double offsetX, offsetY;
	if (!start) {
		_yaw = 270.0f;
		_pitch = 0.0f;
		start = true;
		mouseLastPosX = xPos;
		mouseLastPosY = yPos;
	}
	offsetX = xPos - mouseLastPosX;
	offsetY = yPos - mouseLastPosY;

	mouseLastPosX = xPos;
	mouseLastPosY = yPos;


	offsetX *= sensibility;
	offsetY *= sensibility;

	_yaw += offsetX;
	_pitch -= offsetY;

	_pitch = clamp(_pitch, -89.9f, 89.9f);
	_yaw = mod(_yaw, 360.0f);

	vec3 front;

	front.x = cos(radians(_yaw)) * cos(radians(_pitch));
	front.y = sin(radians(_pitch));
	front.z = sin(radians(_yaw)) * cos(radians(_pitch));
	cameraDir = normalize(front);
}
/*
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (FOV >= 1.0f && FOV <= 60.0f)
		FOV -= yoffset / 10;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 60.0f)
		FOV = 60.0f;
}*/
/*
void DoMovement(GLFWwindow* window) {
	moveBackwards = glfwGetKey(window, GLFW_KEY_W);
	moveForward = glfwGetKey(window, GLFW_KEY_S);
	moveLeft = glfwGetKey(window, GLFW_KEY_A);
	moveRight = glfwGetKey(window, GLFW_KEY_D);
}*/

int main() {
	cameraPos = vec3(0, 0, 3);
	sensibility = 0.04f;
	cameraSpeed = 3.f;
	cameraDir = normalize(vec3(0, 0, 0) - cameraPos);
	cameraRight = normalize(cross(vec3(0, 1, 0), cameraDir));
	cameraUp = normalize(cross(cameraDir, cameraRight));
	cam = Camera(cameraPos, cameraDir, sensibility, FOV);
	mixOp = 0.0f;
	plusRot = 0.25f;

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
	window = glfwCreateWindow(WIDTH, HEIGHT, "Practica1_MarcMolina", NULL, NULL);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetScrollCallback(window, mouseScrollWrapper);
	//que funcion se llama cuando se detecta una pulsaci�n de tecla en la ventana x
	glfwSetKeyCallback(window, key_callback);

	
	//set windows and viewport
	glViewport(0, 0, screenWithd, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	//fondo
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	Shader shad = Shader("./src/textureVertex3d.vertexshader", "./src/textureFragment3d.fragmentshader");
	GLuint vao, vbo/* ebo*/;
	
	//cargamos los shader

	//practica wireframe
	glGenVertexArrays(1, &vao); // Create new VAO
								
	glGenBuffers(1, &vbo); // Create new VBO
						   // Create new buffer that will be used to store indices
	//glGenBuffers(1, &ebo);
			

	glBindVertexArray(vao); {// Binded VAO will store connections between VBOs and attributes
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind vbo as current vertex buffer
											// initialize vertex buffer, allocate memory, fill it with data
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);
		
		/*
		// Bind index buffer to corresponding target

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		// ititialize index buffer, allocate memory, fill it with data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);
		*/

		// indicate that current VBO should be used with vertex attribute with index 0
		glEnableVertexAttribArray(0);
		// indicate how vertex attribute 0 should interpret data in connected VBO
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	
		/*
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 0, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));*/

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}glBindVertexArray(0);
	
	int width, height;
	
	
	//texture
	glGenTextures(2, textures);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate the texture
	unsigned char* image = SOIL_load_image("./src/texture.png", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate the texture
	image = SOIL_load_image("./src/img.png", &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{	
		myLook = cam.LookAt();

		mLook = lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		/*if (myLook == mLook) {
			cout << "Matriz Look Ok \n";
		}*/

		cam.DoMovement(window);

		cam.Deltatime = glfwGetTime() - cam.Lastframe;

		cam.Lastframe = glfwGetTime();

		//comprueba que algun disparador se halla activado (tales como el teclado, raton, etc)
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

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

		glUniform1f(mixID, mixOp);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniform1i(locTex1, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glUniform1i(locTex2, 1);

		//establecer el shader
		shad.USE();	


		//creación de transformaciones

		mat4 view;
		mat4 projection;
		//view = translate(view, cameraPos);
		projection = perspective(radians(cam.GetFOV()), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		view = lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		GLint modelLocation = glGetUniformLocation(shad.Program, "model");
		GLint viewLocation = glGetUniformLocation(shad.Program, "view");
		GLint projLocation = glGetUniformLocation(shad.Program, "projection");

		// paso las matrices al shader de vertices
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, value_ptr(projection));

		glBindVertexArray(vao);

		/*
		if (WIREFRAME) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}*/

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

		if (cam.moveForward) {
			cameraPos.z -= normalize(cameraDir).z*cameraSpeed*cam.Deltatime;
		}
		else if (cam.moveBackwards) {
			cameraPos.z += normalize(cameraDir).z*cameraSpeed*cam.Deltatime;
		}
		if (cam.moveLeft) {
			cameraPos.x += normalize(cameraRight).x*cameraSpeed*cam.Deltatime;
		}
		else if (cam.moveRight) {
			cameraPos.x -= normalize(cameraRight).x*cameraSpeed*cam.Deltatime;
		}

		if (plusRotLeft) {
			rotY -= plusRot;
		}
		else if (plusRotRight) {
			rotY += plusRot;
		}

		if (plusRotUp) {
			rotX -= plusRot;
		}
		else if (plusRotDown) {
			rotX += plusRot;
		}

		for (GLuint i = 0; i < 10; i++)
		{
			// Calculate the model matrix for each object and pass it to shader before drawing
			mat4 model;
			if (i == 0) {
				model = translate(model, CubesPositionBuffer[i]);
				model = rotate(model, radians(rotX), glm::vec3(1.f, 0.f, 0.f));
				model = rotate(model, radians(rotY), glm::vec3(0.f, 1.f, 0.f));
			}
			else {
				float rot = glfwGetTime() * 100;
				rot = (int)rot % 360;
				model = translate(model, vec3(0.0f));
				model = translate(model, CubesPositionBuffer[i]);
				model = rotate(model, radians(rot), vec3(1, 0.5f, 0));
			}

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
		
		/*matrixPlane = translate(matrixPlane, vec3(0.0f, -0.5f, 0.0f));
		matrixPlane = rotate(matrixPlane, radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		matrixPlane = projCamera * posCamera * matrixPlane;
	
		
		glUniformMatrix4fv(matrixPlaneID, 1, GL_FALSE, value_ptr(matrixPlane));


		//pitar el VAO
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);*/

		// reset bindings for VAO
		glBindVertexArray(0);

		//intercambia el framebuffer
		glfwSwapBuffers(window);
		
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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cuando se pulsa una tecla escape cerramos la aplicacion
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	/*
	//modo wireframe
	if (key == GLFW_KEY_W && action == GLFW_PRESS && WIREFRAME == false) {
	WIREFRAME = true;
	}

	//no wireframe
	else if (key == GLFW_KEY_W && action == GLFW_PRESS && WIREFRAME == true) {
	WIREFRAME = false;
	}*/

	if (key == GLFW_KEY_E&&action == GLFW_PRESS) {
		PrintAndCompareMatrix(cam.LookAt(), mLook);
	}

	if (key == GLFW_KEY_1&&action == GLFW_PRESS) {
		fade = true;
	}
	else if (key == GLFW_KEY_2&&action == GLFW_PRESS) {
		fade = false;
	}

	if (key == GLFW_KEY_UP&&action == GLFW_PRESS) {
		plusRotUp = true;
	}

	if (key == GLFW_KEY_UP&&action == GLFW_RELEASE) {
		plusRotUp = false;
	}

	if (key == GLFW_KEY_DOWN&&action == GLFW_PRESS) {
		plusRotDown = true;
	}

	if (key == GLFW_KEY_DOWN&&action == GLFW_RELEASE) {
		plusRotDown = false;
	}

	if (key == GLFW_KEY_RIGHT&&action == GLFW_PRESS) {
		plusRotRight = true;
	}
	else if (key == GLFW_KEY_RIGHT&&action == GLFW_RELEASE) {
		plusRotRight = false;
	}

	if (key == GLFW_KEY_LEFT&&action == GLFW_PRESS) {
		plusRotLeft = true;
	}
	else if (key == GLFW_KEY_LEFT&&action == GLFW_RELEASE) {
		plusRotLeft = false;
	}

	/*
	if (key == GLFW_KEY_LEFT&&action == GLFW_PRESS) {
	directionRotate = 1;
	}
	else if (key == GLFW_KEY_RIGHT&&action == GLFW_PRESS) {
	directionRotate = 2;
	}*/
}
