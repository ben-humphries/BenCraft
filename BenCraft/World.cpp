#include "World.h"

#include "PerlinNoise.hpp"

World::World()
{
	for (int i = 0; i < WORLD_SIZE; i++) {
		for (int j = 0; j < WORLD_HEIGHT; j++) {
			for (int k = 0; k < WORLD_SIZE; k++) {
				Chunk c;
				c.setPosition(glm::vec3(i, j, k));
				chunks.push_back(c);
			}
		}
	}
}


World::~World()
{
}

void World::render(Camera & cam)
{
	for (int i = 0; i < chunks.size(); i++) {
		chunks[i].render(cam);
	}
}

int World::getHeightAtXY(glm::vec2 position)
{
	
	double freq = 4;
	int octaves = 8;
	int waterLevel = 10;

	siv::PerlinNoise noise(12345);

	double fx = worldSizeBlocks / freq, fy = worldSizeBlocks / freq;

	double val = noise.octaveNoise0_1(position.x / fx, position.y / fy, octaves) * worldHeightBlocks;

	return (int) val;
}
