#include "Camera.h"

Camera::Camera(float windowWidth, float windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.1f, 100.0f);

	updateCameraAttributes();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projection;
}

void Camera::setProjectionMatrix(glm::mat4 projectionMatrix)
{
	this->projection = projectionMatrix;
}


void Camera::move(glm::vec3 tomove)
{
	cameraPos += tomove;
	updateCameraAttributes();
}

void Camera::fpKeyboardMove(MoveCamera direction)
{
	float speed = MOVEMENT_SPEED;

	if (direction == RIGHT) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}
	else if (direction == LEFT) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}

	if (direction == UP) {
		cameraPos += glm::vec3(0, 1, 0) * speed;
	}
	else if (direction == DOWN) {
		cameraPos -= glm::vec3(0, 1, 0) * speed;
	}

	if (direction == FORWARD) {
		cameraPos += cameraFront * speed;
	}
	else if (direction == BACKWARD) {
		cameraPos -= cameraFront * speed;
	}

	updateCameraAttributes();

}

void Camera::fpMouseMove(float dx, float dy, bool constrainPitch)
{
	yaw = glm::mod(yaw + dx * MOUSE_SENSITIVITY, 360.0f);
	pitch += dy * MOUSE_SENSITIVITY;


	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		else if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraAttributes();
}

void Camera::updateCameraAttributes() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	glm::vec3 right = glm::normalize(glm::cross(cameraFront, glm::vec3(0,1,0))); 
	cameraUp = glm::normalize(glm::cross(right, cameraFront));
}
