#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include <fstream>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Cube.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const char * loadShader(std::string shaderDir);
void checkShaderCompilation(unsigned int shader);


int main()
{
	//set version of OpenGL
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "LearnOpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	//make the SFML window the current OpenGL context
	window.setActive(true);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//initialize glew (after setting the OpenGL context)
	glewExperimental = GL_TRUE;
	glewInit();

	Cube cube1, cube2;
	cube2.move(glm::vec3(1, 2, -3));

	//texture loading
	sf::Image img;

	if (!img.loadFromFile("texture.png")) {
		std::cout << "Error: could not load texture from file" << std::endl;
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());


	//MODEL VIEW PROJECTION MATRICES
	glm::mat4 model(1);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view(1);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection(1);
	projection = glm::perspective(glm::radians(45.0f), (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

	//Enable z-buffer testing
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	bool running = true;
	bool wireframe = false;
	sf::Clock clock;
	while (running)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space) {
					if (wireframe) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
					else {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					}

					wireframe = !wireframe;
					std::cout << "Toggled wireframe... wireframe enabled: " << wireframe << std::endl;
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		model = glm::rotate(model, glm::radians(-55.0f * clock.getElapsedTime().asSeconds()), glm::vec3(1.0f, 0.5f, 0.3f));

		cube1.rotate(-55.0f * clock.getElapsedTime().asSeconds(), glm::vec3(1.0f, 0.5f, 0.3));
		cube2.rotate(-100.0f * clock.getElapsedTime().asSeconds(), glm::vec3(1.0f, 0.5f, 0.3));

		clock.restart();

		cube1.render(view, projection);
		cube2.render(view, projection);

		window.display();
	}

	window.setActive(false);

	return 0;
}

 const char * loadShader(std::string shaderDir) {
	 std::ifstream t;
	 t.open(shaderDir);
	 t.seekg(0, std::ios::end);
	 int length = t.tellg();
	 t.seekg(0, std::ios::beg);
	 char * buffer = new char[length];
	 t.read(buffer, length);
	 t.close();
	 return buffer;
}

 void checkShaderCompilation(unsigned int shader) {
	 int success;
	 glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	 if (!success) {
		 char infoLog[512];
		 glGetShaderInfoLog(shader, 512, NULL, infoLog);
		 std::cout << "ERROR: Shader Compilation failed...\n" << infoLog << std::endl;
	 }
 }