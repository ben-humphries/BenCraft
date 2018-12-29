#pragma once

#include "Chunk.h"
#include "Camera.h"

const int WORLD_SIZE = 10; //in chunks
const int WORLD_HEIGHT = 2; //in chunks


class World
{
public:
	World();
	~World();

	std::vector<Chunk> chunks;

	void render(Camera & cam);

private:
	long worldSizeBlocks;

};

