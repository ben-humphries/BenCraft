#include "Chunk.h"
#include "Shader.h"

const float frontFace[18] = {
	0.5f, -0.5f, -0.5f, //BL
   -0.5f, -0.5f, -0.5f, //BR
   -0.5f,  0.5f, -0.5f, //TR
   -0.5f,  0.5f, -0.5f, //TR
	0.5f,  0.5f, -0.5f, //TL
	0.5f, -0.5f, -0.5f, //BL -z face
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
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f //-x face
};

const float leftFace[18] = {
	 0.5f,  -0.5f,  0.5f, //BL
	 0.5f,  -0.5f, -0.5f, //BR
	 0.5f,   0.5f, -0.5f, //TR
	 0.5f,   0.5f, -0.5f, //TR
	 0.5f,   0.5f,  0.5f, //TL
	 0.5f,  -0.5f,  0.5f //BL +x face
};

const float bottomFace[18] = {
	 0.5f, -0.5f,  0.5f, //BL
	-0.5f, -0.5f,  0.5f, //BR
	-0.5f, -0.5f, -0.5f, //TR
	-0.5f, -0.5f, -0.5f, //TR
	 0.5f, -0.5f, -0.5f, //TL
	 0.5f, -0.5f,  0.5f //BL -y face
};

const float topFace[18] = {
	 0.5f,  0.5f, -0.5f, //BL
	-0.5f,  0.5f, -0.5f, //BR
	-0.5f,  0.5f,  0.5f, //TR
	-0.5f,  0.5f,  0.5f, //TR
	 0.5f,  0.5f,  0.5f, //TL
	 0.5f,  0.5f, -0.5f //BL +y face
};

float textureCoords[12] = {
	0.0f, 1.0f, //BL
	1.0f, 1.0f, //BR
	1.0f, 0.0f, //TR
	1.0f, 0.0f, //TR
	0.0f, 0.0f, //TL
	0.0f, 1.0f //BL
};

const float TEXTURE_ATLAS_SIZE = 10;

static Shader shader;
bool shaderInitialized = false;

Chunk::Chunk()
{
	initializeBlocks();
	generateMesh();

}


Chunk::~Chunk()
{
}

void Chunk::initializeBlocks()
{
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {

				blocks[i][j][k] = Block(BLOCKTYPE_AIR);

			}
		}
	}
}

void Chunk::generateMesh()
{

	mesh.clear();

	//loop through all blocks in the chunk
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {

				tryAddFace(frontFace, i, j, k, i, j, k - 1, blocks[i][j][k].textureOffsets.frontFace);
				tryAddFace(backFace, i, j, k, i, j, k + 1, blocks[i][j][k].textureOffsets.backFace);
				tryAddFace(leftFace, i, j, k, i + 1, j, k, blocks[i][j][k].textureOffsets.leftFace);
				tryAddFace(rightFace, i, j, k, i - 1, j, k, blocks[i][j][k].textureOffsets.rightFace);
				tryAddFace(bottomFace, i, j, k, i, j - 1, k, blocks[i][j][k].textureOffsets.bottomFace);
				tryAddFace(topFace, i, j, k, i, j + 1, k, blocks[i][j][k].textureOffsets.topFace);

			}
		}
	}
	//test each face by comparing to adjacent block

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
	if (mesh.size() > 0)
		glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), &mesh[0], GL_STATIC_DRAW);


	//link vertex attributes, position first, then color
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Chunk::render(Camera & cam)
{
	glBindVertexArray(vao);

	glm::mat4 trans = cam.getProjectionMatrix() * cam.getViewMatrix() * model;// model;
	shader.setMat4("transform", trans);
	shader.use();

	glDrawArrays(GL_TRIANGLES, 0, mesh.size() / 5); // mesh.size() * 3 / 5 (to get rid of texCoords) then / 3 for numTriangles
}

void Chunk::setPosition(glm::vec3 position)
{
	this->position = position;
	model = glm::mat4(1);

	glm::vec3 b_position = glm::vec3(position.x * CHUNK_SIZE, position.y * CHUNK_SIZE, position.z * CHUNK_SIZE);
	model = glm::translate(model, b_position);
}

int Chunk::getChunkSize()
{
	return CHUNK_SIZE;
}

//adj values should be the coordinates of the block in the direction of the face being added
//for example, if adding rightFace, adj_i = i+1
void Chunk::tryAddFace(const float face[18], int i, int j, int k, int adj_i, int adj_j, int adj_k, int textureOffset) {

	Block current_block = blocks[i][j][k];

	if (current_block.type == BLOCKTYPE_AIR) { //if this block is air, don't add the face
		return;
	}

	if (adj_i < CHUNK_SIZE && //if the adjacent block exists
		adj_j < CHUNK_SIZE &&
		adj_k < CHUNK_SIZE &&
		adj_i >= 0 &&
		adj_j >= 0 &&
		adj_k >= 0) { 
		
		Block adjacent_block = blocks[adj_i][adj_j][adj_k];

		if (adjacent_block.opaque) { //if the adjacent block is opaque, dont add the face
			return;
		}
	}

	//addFace
	addToMesh(face, i, j, k, textureOffset);

}

void Chunk::addToMesh(const float vertices[18], float xOffset, float yOffset, float zOffset, int textureOffset)
{
	float offset_x = textureOffset % 10;
	float offset_y = textureOffset / 10;
	
	offset_x /= TEXTURE_ATLAS_SIZE;
	offset_y /= TEXTURE_ATLAS_SIZE;

	if (offset_x < 0) offset_x = 0;
	if (offset_y < 0) offset_y = 0;

	for (int i = 0; i < 6; i++) {
		mesh.push_back(vertices[3 * i] + xOffset);
		mesh.push_back(vertices[3 * i + 1] + yOffset);
		mesh.push_back(vertices[3 * i + 2] + zOffset);
		

		mesh.push_back(textureCoords[2 * i] / TEXTURE_ATLAS_SIZE + offset_x); //x
		mesh.push_back(textureCoords[2 * i + 1] / TEXTURE_ATLAS_SIZE + offset_y); //y
	}
}



