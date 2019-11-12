#include <SFML/Window.hpp>
#include <GL/glew.h>

#include <fstream>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"
#include "Skybox.h"
#include "World.h"
#include "TextureAtlas.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

void processInput(sf::Window &window, float dt);

//Global variables
sf::ContextSettings settings;
//window is declared in main once the context settings are initialized.
Camera * camera;

int main()
{
	//set version of OpenGL
	settings.depthBits = 24;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	sf::Window window;
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "BenCraft", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	//make the SFML window the current OpenGL context
	window.setActive(true);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//initialize glew (after setting the OpenGL context)
	glewExperimental = GL_TRUE;
	glewInit();

	Skybox skybox;

	//Texture loading
	skybox.bindCubeMapTexture();


	//Enable z-buffer testing
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	//Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.3f, 0.8f, 1.0f);

	camera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	//Util variables
	bool gameFocused = true;
	float elapsedTime = 0.0f;
	bool running = true;
	bool wireframe = false;
	sf::Clock clock;

	//TEMP VARIABLES
	World world = World();
	TextureAtlas textureAtlas; //move this to a world/terrain class eventually?
	///////////
	//TEMP INITIALIZATIONS
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			for (int z = 0; z < 64; z++) {
				if (x > 6 && z > 14)
					world.set(x, y, z, 3);
				else
					world.set(x, y, z, 1);
			}
		}
	}

	for (int x = 64; x < 128; x++) {
		for (int y = 0; y < 64; y++) {
			for (int z = 0; z < 64; z++) {
				world.set(x, y, z, 2);
			}
		}
	}

	for (int x = -128; x < 0; x++) {
		for (int y = -32; y < 32; y++) {
			for (int z = -64; z < 64; z++) {
				world.set(x, y, z, 3);
			}
		}
	}

	world.set(0, 0, 0, 4);
	world.set(-1, 0, 0, 4);
	textureAtlas.load("res/textures/TextureAtlas.png");
	///////////

	skybox.bindCubeMapTexture();
	textureAtlas.bind();

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

		float dt = clock.getElapsedTime().asSeconds();
		elapsedTime += dt;
		clock.restart();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (gameFocused) {
			processInput(window, dt);
		}

		skybox.render(camera);
		world.render(camera);

		glm::vec3 cameraPos = camera->getPosition();
		//printf("Camera Pos: x = %f, y = %f, z = %f\n", cameraPos.x, cameraPos.y, cameraPos.z);

		window.display();
	}
	window.setActive(false);

	return 0;
}


void processInput(sf::Window &window, float dt) {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		camera->fpKeyboardMove(FORWARD, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		camera->fpKeyboardMove(LEFT, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		camera->fpKeyboardMove(BACKWARD, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		camera->fpKeyboardMove(RIGHT, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		camera->fpKeyboardMove(UP, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		camera->fpKeyboardMove(DOWN, dt);
	}

	//to fix beginning offset, set mouse position to centerScreen on init
	sf::Vector2i windowPos = window.getPosition();
	sf::Vector2i centerScreen = sf::Vector2i(windowPos.x + WINDOW_WIDTH / 2, windowPos.y + WINDOW_HEIGHT / 2);

	sf::Vector2i currentPos = sf::Mouse::getPosition() - centerScreen;
	camera->fpMouseMove(currentPos.x, -currentPos.y);
	sf::Mouse::setPosition(centerScreen);
}
