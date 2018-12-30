#pragma once

#include "Chunk.h"
#include "Camera.h"
#include "PerlinNoise.hpp"


const int WORLD_SIZE = 5; //in chunks
const int WORLD_HEIGHT = 1; //in chunks


class World
{
public:
	World();
	~World();

	std::vector<Chunk> chunks;

	void render(Camera & cam);

private:
	long worldSizeBlocks;
	long worldHeightBlocks;

	int getHeightAtXZ(glm::vec2 position);

	int getChunkAt(glm::vec3 position);
};

