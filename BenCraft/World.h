#pragma once

#include "Chunk.h"
#include "Camera.h"
#include "PerlinNoise.hpp"


const int MAX_CHUNKS = 16;
const int SQRT_MAX_CHUNKS = glm::sqrt(MAX_CHUNKS);

class World
{
public:
	World();
	~World();

	std::vector<Chunk> chunks;

	void render(Camera & cam);

	void updateChunks(Camera & cam);

private:
	long worldSizeBlocks;

	int getHeightAtXZ(glm::vec2 position);

	int getChunkAt(glm::vec3 position);

	void loadChunk(glm::vec3 position);
	bool isChunkLoaded(glm::vec3 position);
	void unloadChunk(glm::vec3 position);
	void unloadChunk(int index);

	std::map<float, int> sortChunksByDistanceToCamera(Camera & cam);
};

