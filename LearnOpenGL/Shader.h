#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <iostream>


class Shader 
{
public:
	unsigned int ID;

	Shader(const char * vertexPath, const char * fragmentPath) {

		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		fragment = glCreateShader(GL_FRAGMENT_SHADER);

		const char * vertexShaderSource = loadShader(vertexPath);
		const char * fragShaderSource = loadShader(fragmentPath);

		glShaderSource(vertex, 1, &vertexShaderSource, NULL);
		glCompileShader(vertex);

		checkShaderCompilation(vertex);

		glShaderSource(fragment, 1, &fragShaderSource, NULL);
		glCompileShader(fragment);

		checkShaderCompilation(fragment);

		ID = glCreateProgram();

		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	//Use shader program
	void use() {
		glUseProgram(ID);
	}

	void setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:

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
};

#endif


