//GLEW
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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"
#include "Object.h"


using namespace std;
using namespace glm;

GLFWwindow* window;
const GLuint WIDTH = 800, HEIGHT = 600;

//camara
Camera cam;
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
void PrintAndCompareMatrix(mat4 m1, mat4 m2);
void PrintMatrix(mat4 m);
void cursor_callback(GLFWwindow* window, double xPos, double yPos);
float ambStrength, specStrength;


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
	//cam.MouseMove(window, xPos, yPos);
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

	front.x = cos(radians(_pitch)) * cos(radians(_yaw));
	front.y = sin(radians(_pitch));
	front.z = cos(radians(_pitch)) * sin(radians(_yaw));
	cameraDir = normalize(front);
}

int main() {
	cameraPos = vec3(0, 0, 3);
	sensibility = 0.04f;
	cameraSpeed = 3.f;
	cameraDir = normalize(vec3(0, 0, 0) - cameraPos);
	cameraRight = normalize(cross(vec3(0, 1, 0), cameraDir));
	cameraUp = normalize(cross(cameraDir, cameraRight));
	cam = Camera(cameraPos, cameraDir, sensibility, FOV);
	ambStrength = 0.1f;
	specStrength = 0.5f;

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
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	Shader lightingShader("./src/lighting.vertexshader", "./src/lighting.fragmentshader"); //objeto a iluminar
	Shader lampShader("./src/lamp.vertexshader", "./src/lamp.fragmentshader"); //luz
	Object A = Object(vec3(0.4f, 0.4f, 0.4f), vec3(0.f, 45.f, 0.f), vec3(1.5f, -1.0f, -1.5f), Object::cube);
	Object B = Object(vec3(0.1f, 0.1f, 0.1f), vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, 0.f), Object::cube);

	
	//bucle de dibujado
	while (!glfwWindowShouldClose(window))
	{	
		myLook = cam.LookAt();

		mLook = lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		/*if (myLook == mLook) {
			cout << "Matriz Look Ok \n";
		}*/

		//comprueba que algun disparador se halla activado (tales como el teclado, raton, etc)
		glfwPollEvents();

		cam.DoMovement(window);
		A.DoMoveAndRot(window);

		cam.Deltatime = glfwGetTime() - cam.Lastframe;

		cam.Lastframe = glfwGetTime();

		// Clear the colorbuffer
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//establecer el shader
		lightingShader.USE();	
		vec3 lightPos = B.GetPosition();
		vec3 camPos = cam.GetCamPos();
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		GLint ambStrengthLoc = glGetUniformLocation(lightingShader.Program, "ambStrength");
		GLint specStrengthLoc = glGetUniformLocation(lightingShader.Program, "specStrength");
		glUniform3f(objectColorLoc, 0.8f, 0.6f, 0.4f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
		glUniform1f(ambStrengthLoc, ambStrength);
		glUniform1f(specStrengthLoc, specStrength);


		//creación de transformaciones

		mat4 view;
		mat4 projection;
		//view = translate(view, cameraPos);
		projection = perspective(radians(cam.GetFOV()), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		view = lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		GLint modelLocation = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLocation = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLocation = glGetUniformLocation(lightingShader.Program, "projection");

		// paso las matrices al shader de vertices
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, value_ptr(projection));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(A.GetModelMatrix()));
		A.Draw();

		lampShader.USE();
		modelLocation = glGetUniformLocation(lampShader.Program, "model");
		viewLocation = glGetUniformLocation(lampShader.Program, "view");
		projLocation = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, value_ptr(projection));

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(B.GetModelMatrix()));
		B.Draw();

		if (cam.moveForward) {
			cameraPos -= cameraDir*(cameraSpeed*cam.Deltatime);
		}
		if (cam.moveBackwards) {
			cameraPos += cameraDir*(cameraSpeed*cam.Deltatime);
		}
		if (cam.moveLeft) {
			cameraPos.x += normalize(cameraRight).x*(cameraSpeed*cam.Deltatime);
		}
		if (cam.moveRight) {
			cameraPos.x -= normalize(cameraRight).x*(cameraSpeed*cam.Deltatime);
		}
		A.MoveAndRot(cam.Deltatime);
	
		//intercambia el framebuffer
		glfwSwapBuffers(window);
		
	}

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
}
