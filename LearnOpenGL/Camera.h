#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
public:

	Camera(float windowWidth, float windowHeight);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void setViewMatrix(glm::mat4 viewMatrix);
	void setProjectionMatrix(glm::mat4 projectionMatrix);

	void lookAt(glm::vec3 target);

	//first person movement
	void move(glm::vec3 tomove);



private:

	glm::mat4 view = glm::mat4(1);
	glm::mat4 projection = glm::mat4(1);

	float windowWidth, windowHeight;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif