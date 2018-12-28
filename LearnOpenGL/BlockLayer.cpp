#include "BlockLayer.h"

static unsigned int vao;
static Shader shader;
bool shaderInitialized = false;

BlockLayer::BlockLayer()
{
	generateMesh();

	if (!shaderInitialized) {
		shader = Shader("vertex.glsl", "fragment.glsl");
		shaderInitialized = true;
	}

	if (vao == 0)
		glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	//initialize vertex buffer object
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//copy vertices data to vbo
	glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &mesh[0], GL_STATIC_DRAW);


	//link vertex attributes, position first, then color
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}


BlockLayer::~BlockLayer()
{
}

void BlockLayer::generateMesh()
{


	for (int i = 0; i < LAYER_SIZE - 1; i++) {
		for (int j = 0; j < LAYER_SIZE - 1; j++) {
			if (blocks[i + 1][j].opaque) { //check block to the right
				addToMesh(blocks[i][j].rightFace, blocks[i][j].textureCoords, i, j);
				addToMesh(blocks[i+1][j].leftFace, blocks[i][j].textureCoords, i+1, j);
			}
			if (blocks[i][j + 1].opaque) { //check block behind
				addToMesh(blocks[i][j].backFace, blocks[i][j].textureCoords, i, j);
				addToMesh(blocks[i][j+1].frontFace, blocks[i][j].textureCoords, i, j+1);

			}

			addToMesh(blocks[i][j].topFace, blocks[i][j].textureCoords, i, j);
			addToMesh(blocks[i][j].bottomFace, blocks[i][j].textureCoords, i, j);
		}
	}

	/*for (int i = 0; i < mesh.size(); i++) {
		if (i % 5) {
			printf("\n");
		}
		if( mesh.at(i) > 1)
			printf("%f ", mesh.at(i));
	}*/
}

void BlockLayer::render(Camera & cam)
{
	glBindVertexArray(vao);

	glm::mat4 trans = cam.getProjectionMatrix() * cam.getViewMatrix() * glm::mat4(1);// model;
	shader.setMat4("transform", trans);
	shader.use();

	glDrawArrays(GL_TRIANGLES, 0, mesh.size() / 5); // mesh.size() * 3 / 5 (to get rid of texCoords) then / 3 for numTriangles
}

void BlockLayer::addToMesh(float vertices[18], float textureCoords[12], float xOffset, float zOffset)
{
	for (int i = 0; i < 6; i++) {
		mesh.push_back(vertices[3*i] * xOffset);
		mesh.push_back(vertices[3*i + 1]);
		mesh.push_back(vertices[3*i + 2] * zOffset);

		mesh.push_back(textureCoords[2 * i]);
		mesh.push_back(textureCoords[2 * i + 1]);

	}
}
