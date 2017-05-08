#pragma once
#define GLEW_STATIC

#include <glm.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 direction, GLfloat sensitivity, GLfloat fov);
	void DoMovement(GLFWwindow *window);
	void MouseMove(GLFWwindow *window, double xpos, double ypos);
	void MouseScroll(GLFWwindow *window, double xScroll, double yScroll);
	glm::mat4 LookAt();
	glm::vec3 GetCamPos();
	GLfloat GetFOV();
	GLboolean moveForward, moveBackwards, moveLeft, moveRight;
	float Deltatime;
	float Lastframe;
	~Camera();
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	double LastMx;
	double LastMy;
	GLfloat Sensitivity;
	GLboolean firstMouse;
	GLfloat PITCH;
	GLfloat YAW, FOV;
};

