#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
	//set version of OpenGL
	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;

	sf::Window window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "LearnOpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	//make the SFML window the current OpenGL context
	window.setActive(true);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	bool running = true;
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
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.display();
	}

	window.setActive(false);

	return 0;
}