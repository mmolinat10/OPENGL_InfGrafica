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
int selectionLight;


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
	selectionLight = 1;
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

	Shader lightingShader("./src/lighting.vertexshader", "./src/lighting.fragmentshader"); //total phong
	Shader directionalShader("./src/lighting.vertexshader", "./src/directional.fragmentshader"); //directional
	Shader pointLightShader("./src/lighting.vertexshader", "./src/point.fragmentshader"); //point light
	Shader spotLightShader("./src/lighting.vertexshader", "./src/spotLight.fragmentshader"); //point light
	Shader lampShader("./src/lamp.vertexshader", "./src/lamp.fragmentshader"); //luz
	Object A = Object(vec3(0.4f, 0.4f, 0.4f), vec3(0.f, 45.f, 0.f), vec3(1.5f, -0.8f, 0.f), Object::cube);
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

		vec3 lightPos = B.GetPosition();
		vec3 camPos = cam.GetCamPos();
		vec3 camFront = cam.GetCamFront();
		vec3 camRight = cam.GetCamRight();
		vec3 camUp = cam.GetCamUp();
		
		//totalPhong
		if (selectionLight == 1) {
			//establecer el shader
			lightingShader.USE();

			GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
			GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
			GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
			GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
			GLint ambStrengthLoc = glGetUniformLocation(lightingShader.Program, "ambStrength");
			GLint specStrengthLoc = glGetUniformLocation(lightingShader.Program, "specStrength");
			glUniform3f(objectColorLoc, 0.8f, 0.6f, 0.4f);
			glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
			glUniform1f(ambStrengthLoc, ambStrength);
			glUniform1f(specStrengthLoc, specStrength);
		}
		
		//directionalLight
		if (selectionLight == 2) {
			//establecer el shader
			directionalShader.USE();
			GLint lightDirLoc = glGetUniformLocation(directionalShader.Program, "light.direction");
			GLint viewPosLoc = glGetUniformLocation(directionalShader.Program, "viewPos");
			GLint objectColorLoc = glGetUniformLocation(directionalShader.Program, "objectColor");
			GLint lightColorLoc = glGetUniformLocation(directionalShader.Program, "lightColor");
			glUniform3f(objectColorLoc, 0.8f, 0.6f, 0.4f);
			glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

			glUniform3f(lightDirLoc, 0.0f, -1.0f, 0.f);
			glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
			// Set lights properties
			glUniform3f(glGetUniformLocation(directionalShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(directionalShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(directionalShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		}

		//pointLight
		if (selectionLight == 3) {
			pointLightShader.USE();
			GLint lightPosLoc = glGetUniformLocation(pointLightShader.Program, "light.position");
			GLint viewPosLoc = glGetUniformLocation(pointLightShader.Program, "viewPos");
			GLint objectColorLoc = glGetUniformLocation(pointLightShader.Program, "objectColor");
			GLint lightColorLoc = glGetUniformLocation(pointLightShader.Program, "lightColor");
			glUniform3f(objectColorLoc, 0.8f, 0.6f, 0.4f);
			glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
			// Set lights properties
			glUniform3f(glGetUniformLocation(pointLightShader.Program, "light.ambient"), 0.2f, 0.2f, 0.2f);
			glUniform3f(glGetUniformLocation(pointLightShader.Program, "light.diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(pointLightShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(pointLightShader.Program, "light.constant"), 1.0f);
			glUniform1f(glGetUniformLocation(pointLightShader.Program, "light.linear"), 0.09);
			glUniform1f(glGetUniformLocation(pointLightShader.Program, "light.quadratic"), 0.032);
		}

		//spotLight
		if (selectionLight == 4) {
			spotLightShader.USE();
			GLint lightPosLoc = glGetUniformLocation(spotLightShader.Program, "light.position");
			GLint lightSpotdirLoc = glGetUniformLocation(spotLightShader.Program, "light.direction");
			GLint lightSpotCutOffLoc = glGetUniformLocation(spotLightShader.Program, "light.cutOff");
			GLint lightSpotOuterCutOffLoc = glGetUniformLocation(spotLightShader.Program, "light.outerCutOff");
			GLint viewPosLoc = glGetUniformLocation(spotLightShader.Program, "viewPos");
			GLint objectColorLoc = glGetUniformLocation(spotLightShader.Program, "objectColor");
			GLint lightColorLoc = glGetUniformLocation(spotLightShader.Program, "lightColor");
			glUniform3f(objectColorLoc, 0.8f, 0.6f, 0.4f);
			glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

			glUniform3f(lightPosLoc, camPos.x, camPos.y, camPos.z);
			glUniform3f(lightSpotdirLoc, camFront.x, camFront.y, camFront.z);
			glUniform1f(lightSpotCutOffLoc, glm::cos(glm::radians(14.f)));
			glUniform1f(lightSpotOuterCutOffLoc, glm::cos(glm::radians(20.f)));
			glUniform3f(viewPosLoc, camPos.x, camPos.y, camPos.z);
			// Set lights properties
			glUniform3f(glGetUniformLocation(spotLightShader.Program, "light.ambient"), 0.1f, 0.1f, 0.1f);
			glUniform3f(glGetUniformLocation(spotLightShader.Program, "light.diffuse"), 0.8f, 0.8f, 0.8f);
			glUniform3f(glGetUniformLocation(spotLightShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(spotLightShader.Program, "light.constant"), 1.0f);
			glUniform1f(glGetUniformLocation(spotLightShader.Program, "light.linear"), 0.09);
			glUniform1f(glGetUniformLocation(spotLightShader.Program, "light.quadratic"), 0.032);
		}

		//creación de transformaciones

		mat4 view;
		mat4 projection;
		//view = translate(view, cameraPos);
		projection = perspective(radians(cam.GetFOV()), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		view = lookAt(cameraPos, cameraPos + cameraDir, cameraUp);

		GLint modelLocation, viewLocation, projLocation;
		//totalPhong
		if (selectionLight == 1) {
			modelLocation = glGetUniformLocation(lightingShader.Program, "model");
			viewLocation = glGetUniformLocation(lightingShader.Program, "view");
			projLocation = glGetUniformLocation(lightingShader.Program, "projection");

		}
		//directionalLight
		if (selectionLight == 2) {
			modelLocation = glGetUniformLocation(directionalShader.Program, "model");
			viewLocation = glGetUniformLocation(directionalShader.Program, "view");
			projLocation = glGetUniformLocation(directionalShader.Program, "projection");
		}
		//pointLight
		if (selectionLight == 3) {
			modelLocation = glGetUniformLocation(pointLightShader.Program, "model");
			viewLocation = glGetUniformLocation(pointLightShader.Program, "view");
			projLocation = glGetUniformLocation(pointLightShader.Program, "projection");
		}
		//spotLight
		if (selectionLight == 4) {
			modelLocation = glGetUniformLocation(spotLightShader.Program, "model");
			viewLocation = glGetUniformLocation(spotLightShader.Program, "view");
			projLocation = glGetUniformLocation(spotLightShader.Program, "projection");
		}
		
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

		//si es diferente a directional light muestrame el cubo
		if (selectionLight != 2) {
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(B.GetModelMatrix()));
			B.Draw();
		}

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

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		selectionLight = 1;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		selectionLight = 2;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		selectionLight = 3;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
		selectionLight = 4;
	}
}
