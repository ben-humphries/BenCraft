#include "BlockLayer.h"

static unsigned int vao;
static Shader shader;
bool shaderInitialized = false;

BlockLayer::BlockLayer()
{

	for (int i = 0; i < LAYER_SIZE*LAYER_SIZE; i++) {
			blocks.push_back(Block());
			blocks[i].opaque = true;
	}
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


	for (int i = 0; i < LAYER_SIZE*LAYER_SIZE - LAYER_SIZE; i++) {
		/*if (i == 0) {
			addToMesh(blocks[i][j].rightFace, blocks[i][j].textureCoords, i, j);
		}
		if (j == 0) {
			addToMesh(blocks[i][j].frontFace, blocks[i][j].textureCoords, i, j);
		}*/

		printf("%i", i);
		if (!blocks[i + 1].opaque) { //block to the right
			addToMesh(blocks[i].rightFace, blocks[i].textureCoords, i % LAYER_SIZE, i / LAYER_SIZE);
			addToMesh(blocks[i + 1].leftFace, blocks[i + 1].textureCoords, i + 1 % LAYER_SIZE, i + 1 / LAYER_SIZE);
		}

		if (!blocks[i + LAYER_SIZE].opaque) { //block behind
			addToMesh(blocks[i].backFace, blocks[i].textureCoords, i % LAYER_SIZE, i / LAYER_SIZE);
			addToMesh(blocks[i + LAYER_SIZE].frontFace, blocks[i + LAYER_SIZE].textureCoords, i % LAYER_SIZE, i + LAYER_SIZE / LAYER_SIZE);
		}


		addToMesh(blocks[i].topFace, blocks[i].textureCoords, i % LAYER_SIZE, i / LAYER_SIZE);
		addToMesh(blocks[i].bottomFace, blocks[i].textureCoords, i % LAYER_SIZE, i / LAYER_SIZE);
	}

	/*for (int i = 0; i < LAYER_SIZE; i++) {
		addToMesh(blocks[i][LAYER_SIZE-1].backFace, blocks[i][LAYER_SIZE-1].textureCoords, i, LAYER_SIZE-1);
		addToMesh(blocks[i][LAYER_SIZE - 1].topFace, blocks[i][LAYER_SIZE - 1].textureCoords, i, LAYER_SIZE - 1);
		addToMesh(blocks[i][LAYER_SIZE - 1].bottomFace, blocks[i][LAYER_SIZE - 1].textureCoords, i, LAYER_SIZE - 1);


	}*/

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
		mesh.push_back(vertices[3*i] + xOffset);
		mesh.push_back(vertices[3*i + 1]);
		mesh.push_back(vertices[3*i + 2] + zOffset);

		mesh.push_back(textureCoords[2 * i]);
		mesh.push_back(textureCoords[2 * i + 1]);

	}
}
