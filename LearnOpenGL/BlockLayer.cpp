#include "BlockLayer.h"

static unsigned int vao;
static Shader shader;
bool shaderInitialized = false;

BlockLayer::BlockLayer()
{

	for (int i = 0; i < LAYER_SIZE*LAYER_SIZE; i++) {
			blocks.push_back(Block());
			blocks[i].opaque = i % 2 == 0;
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


	for (int i = 0; i < LAYER_SIZE*LAYER_SIZE; i++) {

		bool renderCurrent = blocks[i].opaque;
		
		bool checkRight = true;
		bool checkBehind = true;
		
		int x = i % LAYER_SIZE;
		int z = i / LAYER_SIZE;

		int r_i = i + 1;
		int b_i = i + LAYER_SIZE;

		if (i >= LAYER_SIZE*LAYER_SIZE - LAYER_SIZE) {
			if(renderCurrent) addToMesh(blocks[i].backFace, blocks[i].textureCoords, x, z);
			checkBehind = false;

			if (i == LAYER_SIZE * LAYER_SIZE - 1) {
				checkRight = false;
			}
		}

		if ((i + 1) % LAYER_SIZE == 0) { //left edge
			if (renderCurrent) addToMesh(blocks[i].leftFace, blocks[i].textureCoords, x, z);
		}

		if (x == 0) {
			if (renderCurrent) addToMesh(blocks[i].rightFace, blocks[i].textureCoords, x, z);
		}

		if (z == 0) { //front edge
			if (renderCurrent) addToMesh(blocks[i].frontFace, blocks[i].textureCoords, x, z);
		}
		
		if (checkRight && (!blocks[i + 1].opaque)) { //block to the right
			int r_x = (i + 1) % LAYER_SIZE;
			int r_z = (i + 1) / LAYER_SIZE;

			if (renderCurrent) addToMesh(blocks[i].rightFace, blocks[i].textureCoords, x, z);
			addToMesh(blocks[r_i].leftFace, blocks[r_i].textureCoords, r_x, r_z);
			
			/*if (i + 1 % LAYER_SIZE == 0) {
				printf("here");
				addToMesh(blocks[i].leftFace, blocks[i].textureCoords, x, z);
			}*/
		}


		if (checkBehind && !blocks[i + LAYER_SIZE].opaque) { //block behind
			int b_x = (i + LAYER_SIZE) % LAYER_SIZE;
			int b_z = (i + LAYER_SIZE) / LAYER_SIZE;

			if (renderCurrent) addToMesh(blocks[i].backFace, blocks[i].textureCoords, x, z);
			addToMesh(blocks[b_i].frontFace, blocks[b_i].textureCoords, b_x, b_z);
		}


		if (renderCurrent) addToMesh(blocks[i].topFace, blocks[i].textureCoords, x, z);
		if (renderCurrent) addToMesh(blocks[i].bottomFace, blocks[i].textureCoords, x, z);
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
