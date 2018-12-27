#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include <fstream>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL


#include "Cube.h"
#include "Camera.h"



#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void processInput();

Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

bool gameFocused = true;

int main()
{
	//set version of OpenGL
	sf::ContextSettings settings;
	settings.depthBits = 32;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	sf::Window window;


	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "LearnOpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	//make the SFML window the current OpenGL context
	window.setActive(true);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//initialize glew (after setting the OpenGL context)
	glewExperimental = GL_TRUE;
	glewInit();


	Cube cube1;
	Cube cube2;
	cube2.move(glm::vec3(1, 0, 0));

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


	//Enable z-buffer testing
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	//Enable alpha blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.3f, 0.8f, 1.0f);

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
				if (event.key.code == sf::Keyboard::M) {
					if (wireframe) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
					else {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					}

					wireframe = !wireframe;
					std::cout << "Toggled wireframe... wireframe enabled: " << wireframe << std::endl;
				}
				else if (event.key.code == sf::Keyboard::Escape) {
					gameFocused = !gameFocused;
					window.setMouseCursorVisible(!gameFocused);

				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (gameFocused) {
			processInput();
		}

		clock.restart();

		glBindTexture(GL_TEXTURE_2D, 0);
		cube1.render(camera);
		glBindTexture(GL_TEXTURE_2D, texture);
		cube2.render(camera);




		window.display();
	}

	window.setActive(false);

	return 0;
}


void processInput() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		camera.fpKeyboardMove(FORWARD);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		camera.fpKeyboardMove(LEFT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		camera.fpKeyboardMove(BACKWARD);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		camera.fpKeyboardMove(RIGHT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		camera.fpKeyboardMove(UP);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		camera.fpKeyboardMove(DOWN);
	}

	//to fix beginning offset, set mouse position to centerScreen on init
	//sf::Vector2i windowPos = window.getPosition();
	//sf::Vector2i centerScreen = sf::Vector2i(windowPos.x + WINDOW_WIDTH / 2, windowPos.y + WINDOW_HEIGHT / 2);

	//sf::Vector2i currentPos = sf::Mouse::getPosition() - centerScreen;
	//camera.fpMouseMove(currentPos.x, -currentPos.y);
	//sf::Mouse::setPosition(centerScreen);
}
