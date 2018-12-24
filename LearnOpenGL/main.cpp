#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>

#include <fstream>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const char * loadShader(std::string shaderDir);
void checkShaderCompilation(unsigned int shader);


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

	//initialize glew (after setting the OpenGL context)
	glewExperimental = GL_TRUE;
	glewInit();

	//initialize vertices
	float vertices[] = {
		//positions			//colors		//tex coords
	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,// top right
	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
	-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


	//initialize and bind vertex array object 
	//(basically an overarching object that we can rebind later to draw the triangle)
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	
	glBindVertexArray(vao);

	//initialize vertex buffer object
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//copy vertices data to vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//initialize element buffer object
	unsigned int ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Create shader program
	Shader shader = Shader("vertex.glsl", "fragment.glsl");

	//link vertex attributes, position first, then color
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

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


	bool running = true;

	bool wireframe = false;
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

		shader.use();
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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