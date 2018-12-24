#include <SFML/Window.hpp>
#include <GL/glew.h>

#include <fstream>
#include <string>
#include <iostream>

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
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
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

	//load and compile shaders

	const char * vertexShaderSource = loadShader("vertex.glsl");
	const char * fragShaderSource = loadShader("fragment.glsl");

	//vertex

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//check for compile errors
	checkShaderCompilation(vertexShader);

	//fragment
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragShader);

	checkShaderCompilation(fragShader);

	//link shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	//cleanup shader objects after creating shader program

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	//link vertex attributes (in this case, just position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



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

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		window.display();
	}

	window.setActive(false);

	return 0;
}

 const char * loadShader(std::string shaderDir) {
	 std::ifstream ifs(shaderDir);

	 return std::string((std::istreambuf_iterator<char>(ifs)),
		 (std::istreambuf_iterator<char>())).c_str();
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