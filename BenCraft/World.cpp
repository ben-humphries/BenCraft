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

	worldSizeBlocks = WORLD_SIZE * Chunk::getChunkSize();
	worldHeightBlocks = WORLD_HEIGHT * Chunk::getChunkSize();

	//loop through all x and z
	//get the height at the xz
	//start at maxheight -1, fill with air until height
	//after height, fill with dirt.

	for (int x = 0; x < worldSizeBlocks; x++) {
		for (int z = 0; z < worldSizeBlocks; z++) {
			int h = getHeightAtXZ(glm::vec2(x, z));

			//get chunk that x z is in
			int chunkSize = Chunk::getChunkSize();

			int c_x, c_z;
			c_x = x / chunkSize;
			c_z = z / chunkSize;

			int index = getChunkAt(glm::vec3(c_x, 0, c_z));

			for (int y = 0; y < h; y++) {
				chunks[index].blocks[x % chunkSize][y][z % chunkSize].setType(BLOCKTYPE_DIRT);
			}
			for (int y = h-1; y < h; y++) {
				chunks[index].blocks[x % chunkSize][y][z % chunkSize].setType(BLOCKTYPE_GRASS);
			}


		}
	}

	for (int i = 0; i < chunks.size(); i++) {
		chunks[i].generateMesh();
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

int World::getHeightAtXZ(glm::vec2 position)
{
	
	double freq = 4;
	int octaves = 2;
	int waterLevel = 10;

	siv::PerlinNoise noise(12345);

	double fx = worldSizeBlocks / freq, fz = worldSizeBlocks / freq;

	double val = noise.octaveNoise0_1(position.x / fx, position.y / fz, octaves) * worldHeightBlocks;

	return (int) val;
}

int World::getChunkAt(glm::vec3 position)
{
	int chunkSize = Chunk::getChunkSize();
	for (int i = 0; i < chunks.size(); i++) {

		if (position == chunks[i].position) {
			return i;
		}
	}

	return -1;
}
