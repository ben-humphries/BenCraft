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

#define GLT_IMPLEMENTATION
#include "gltext.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

void processInput(sf::RenderWindow &window, float dt);
void drawDebugText(sf::RenderWindow &window, Camera * camera, float fps);

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

	sf::RenderWindow window;
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "BenCraft", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	//make the SFML window the current OpenGL context
	window.setActive(true);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//initialize glew (after setting the OpenGL context)
	glewExperimental = GL_TRUE;
	glewInit();

	gltInit();

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
	float fps = 0.0f;

	//TEMP VARIABLES
	World world = World();
	TextureAtlas textureAtlas; //move this to a world/terrain class eventually?

	world.set(0, 0, 0, 4);
	world.set(1, 0, 0, 2);
	world.set(0, 1, 0, 3);
	world.set(0, 0, 1, 4);
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

		if (fmod(elapsedTime, 1.0f) <= 0.1)
			fps = 1 / dt;
		clock.restart();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (gameFocused) {
			processInput(window, dt);
		}

		skybox.render(camera);
		world.render(camera);

		drawDebugText(window, camera, fps);

		window.display();
	}
	window.setActive(false);

	return 0;
}


void processInput(sf::RenderWindow &window, float dt) {

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

void drawDebugText(sf::RenderWindow &window, Camera * camera, float fps) {


	char text_string1[128];
	snprintf(text_string1, sizeof(text_string1), "Position:\n  x = %f\n  y = %f\n  z = %f", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
	
	char text_string2[10];
	snprintf(text_string2, sizeof(text_string2), "FPS: %f", fps);


	window.pushGLStates();

	GLTtext * text1 = gltCreateText();
	gltSetText(text1, text_string1);

	GLTtext * text2 = gltCreateText();
	gltSetText(text2, text_string2);

	gltBeginDraw();
	gltColor(1.0f, 1.0f, 1.0f, 1.0f);
	gltDrawText2D(text1, 0.0f, 0.0f, 2.0f);
	gltDrawText2D(text2, WINDOW_WIDTH - 150.0f, 0.0f, 2.0f);


	gltEndDraw();
	gltDeleteText(text1);
	gltDeleteText(text2);

	window.popGLStates();
}
