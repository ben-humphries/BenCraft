#pragma once

#include <vector>

#include "Block.h"
#include "Camera.h"

const int CHUNK_SIZE = 16;

class Chunk
{
public:
	Chunk();
	~Chunk();

	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

	void initializeBlocks();
	//void addBlock(BlockType type, )
	void generateMesh();
	void render(Camera &cam);
	void setPosition(glm::vec3 position);

	glm::vec3 position;

	static int getChunkSize();

private:

	void tryAddFace(const float face[18], int i, int j, int k, int adj_i, int adj_j, int adj_k, int textureOffset);

	std::vector<float> mesh;

	glm::mat4 model = glm::mat4(1);

	void addToMesh(const float vertices[18], float xOffset, float yOffset, float zOffset, int textureOffset);

};

