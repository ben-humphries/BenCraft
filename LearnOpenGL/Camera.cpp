#include "Camera.h"

Camera::Camera(float windowWidth, float windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	view = glm::translate(view, cameraPos);
	projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);

}

glm::mat4 Camera::getViewMatrix()
{
	return view;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projection;
}

void Camera::setViewMatrix(glm::mat4 viewMatrix)
{
	this->view = viewMatrix;
}

void Camera::setProjectionMatrix(glm::mat4 projectionMatrix)
{
	this->projection = projectionMatrix;
}

void Camera::lookAt(glm::vec3 target)
{
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::move(glm::vec3 tomove)
{

	float speed = 0.05;

	if (tomove.x == 1) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}
	else if (tomove.x == -1) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}

	if (tomove.y == 1) {
		cameraPos -= glm::vec3(0, 1, 0) * speed;
	}
	else if (tomove.y == -1) {
		cameraPos += glm::vec3(0, 1, 0) * speed;
	}

	if (tomove.z == 1) {
		cameraPos -= cameraFront * speed;
	}
	else if (tomove.z == -1) {
		cameraPos += cameraFront * speed;
	}

	view = glm::mat4(1);
	view = glm::translate(view, cameraPos);

}
