#include "Chunk.h"
#include "Shader.h"

const float frontFace[18] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f //-z face
};

const float backFace[18] = {
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f //+z face
};

const float rightFace[18] = {
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f //+x face
};

const float leftFace[18] = {
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f //-x face
};

const float bottomFace[18] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f //-y face
};

const float topFace[18] = {
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f //+y face
};

const float textureCoords[12] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

static unsigned int vao;
static Shader shader;
bool shaderInitialized = false;

Chunk::Chunk()
{
	initializeBlocks();
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


Chunk::~Chunk()
{
}

void Chunk::initializeBlocks()
{
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				blocks[i][j][k] = Block(BLOCKTYPE_SLIME);
			}
		}
	}
}

void Chunk::generateMesh()
{
	//loop through all blocks in the chunk
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				
				tryAddFace(frontFace, i, j, k, i, j, k - 1);
				//tryAddFace(backFace, i, j, k, i, j, k + 1);

			}
		}
	}
	//test each face by comparing to adjacent block
}

void Chunk::render(Camera & cam)
{
	glBindVertexArray(vao);

	glm::mat4 trans = cam.getProjectionMatrix() * cam.getViewMatrix() * glm::mat4(1);// model;
	shader.setMat4("transform", trans);
	shader.use();

	glDrawArrays(GL_TRIANGLES, 0, mesh.size() / 5); // mesh.size() * 3 / 5 (to get rid of texCoords) then / 3 for numTriangles
}

//adj values should be the coordinates of the block in the direction of the face being added
//for example, if adding rightFace, adj_i = i+1
void Chunk::tryAddFace(const float face[18], int i, int j, int k, int adj_i, int adj_j, int adj_k) {

	Block current_block = blocks[i][j][k];

	if (current_block.type == BLOCKTYPE_AIR) { //if this block is air, don't add the face
		return;
	}

	if (adj_i < CHUNK_SIZE && //if the adjacent block exists
		adj_j < CHUNK_SIZE &&
		adj_k < CHUNK_SIZE) { 
		
		Block adjacent_block = blocks[adj_i][adj_j][adj_k];

		if (adjacent_block.opaque) { //if the adjacent block is opaque, dont add the face
			return;
		}
	}

	//addFace
	addToMesh(face, i, j, k);

}

void Chunk::addToMesh(const float vertices[18], float xOffset, float yOffset, float zOffset)
{
	for (int i = 0; i < 6; i++) {
		mesh.push_back(vertices[3 * i] + xOffset);
		mesh.push_back(vertices[3 * i + 1] + yOffset);
		mesh.push_back(vertices[3 * i + 2] + zOffset);

		mesh.push_back(textureCoords[2 * i]);
		mesh.push_back(textureCoords[2 * i + 1]);
	}
}



