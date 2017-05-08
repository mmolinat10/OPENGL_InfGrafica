#include "Camera.h"


Camera::Camera() {

}


Camera::Camera(glm::vec3 position, glm::vec3 direction, GLfloat sensitivity, GLfloat fov)
{
	cameraPos = position;
	cameraFront = direction;
	Sensitivity = sensitivity;
	FOV = fov;
	firstMouse = false;
	cameraRight = glm::normalize(cross(glm::vec3(0, 1, 0), direction));
	cameraUp = glm::normalize(cross(direction, cameraRight));
	moveForward = moveBackwards = moveRight = moveLeft = false;
	PITCH = 0;
	Lastframe = 0;
}


Camera::~Camera()
{
}

GLfloat Camera::GetFOV() {
	return FOV;
}

glm::vec3 Camera::GetCamPos() {
	return cameraPos;
}

glm::vec3 Camera::GetCamFront() {
	return cameraFront;
}

glm::vec3 Camera::GetCamRight() {
	return cameraRight;
}

glm::vec3 Camera::GetCamUp() {
	return cameraUp;
}

void Camera::DoMovement(GLFWwindow *window) {
	moveBackwards = glfwGetKey(window, GLFW_KEY_W);
	moveForward = glfwGetKey(window, GLFW_KEY_S);
	moveLeft = glfwGetKey(window, GLFW_KEY_A);
	moveRight = glfwGetKey(window, GLFW_KEY_D);

}

void Camera::MouseMove(GLFWwindow *window, double xPos, double yPos) {
	double offsetX, offsetY;
	if (!firstMouse) {
		YAW = 270.0f;
		PITCH = 0.0f;
		firstMouse = true;
		LastMx = xPos;
		LastMy = yPos;
	}
	offsetX = xPos - LastMx;
	offsetY = yPos - LastMy;

	LastMx = xPos;
	LastMy = yPos;


	offsetX *= Sensitivity;
	offsetY *= Sensitivity;

	YAW += offsetX;
	PITCH -= offsetY;

	PITCH = glm::clamp(PITCH, -89.9f, 89.9f);
	YAW = glm::mod(YAW, 360.0f);

	glm::vec3 front;

	front.x = cos(glm::radians(PITCH)) * cos(glm::radians(YAW));
	front.y = sin(glm::radians(PITCH));
	front.z = cos(glm::radians(PITCH)) * sin(glm::radians(YAW));
	cameraFront = normalize(front);

}

void Camera::MouseScroll(GLFWwindow *window, double xScroll, double yScroll) {
	if (FOV >= 1.0f && FOV <= 60.0f)
		FOV -= yScroll / 2;
	if (FOV <= 1.0f)
		FOV = 1.0f;
	if (FOV >= 60.0f)
		FOV = 60.0f;
}

glm::mat4 Camera::LookAt() {
	glm::vec3 zaxis = glm::normalize(cameraPos - (cameraPos + cameraFront));

	glm::vec3 xaxis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), zaxis));

	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	glm::mat4 translation, rotation;
	translation[3][0] = -cameraPos.x;
	translation[3][1] = -cameraPos.y;
	translation[3][2] = -cameraPos.z;
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
}