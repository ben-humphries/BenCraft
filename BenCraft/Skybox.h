#pragma once

#include "Camera.h"
#include "Shader.h"

#include <iostream>

class Skybox {

	std::string faces[6] = {
		"skybox/right.tga",
		"skybox/left.tga",
		"skybox/top.tga",
		"skybox/bottom.tga",
		"skybox/front.tga",
		"skybox/back.tga"
	};

	const float vertices[108] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

public:

	Skybox() {
		loadCubeMap();
	}

	void loadCubeMap() {

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i = 0; i < 6; i++) {

			sf::Image img;

			if (!img.loadFromFile(faces[i])) {
				std::cout << "Error: could not load texture from file" << std::endl;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img.getSize().x, img.getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, img.getPixelsPtr());

		}

		if (vao == 0)
			glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);

		//initialize vertex buffer object
		unsigned int vbo;
		glGenBuffers(1, &vbo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//copy vertices data to vbo
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		//link vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//Initialize shader
		shader = Shader("skybox_vertex.glsl", "skybox_fragment.glsl");

	}
	void bindCubeMapTexture() {
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	}
	void render(Camera & cam) {

		glDepthMask(GL_FALSE);

		glBindVertexArray(vao);

		shader.setMat4("projection", cam.getProjectionMatrix());
		shader.setMat4("view", cam.getViewMatrix());

		shader.use();

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthMask(GL_TRUE);

	}

private:

	unsigned int texture;
	unsigned int vao;

	Shader shader;
};