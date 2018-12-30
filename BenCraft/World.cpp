#include "World.h"

#include "PerlinNoise.hpp"

World::World()
{
	for (int i = 0; i < WORLD_SIZE; i++) {
		for (int k = 0; k < WORLD_SIZE; k++) {
			Chunk c;
			c.setPosition(glm::vec3(i, 0, k));
			chunks.push_back(c);
		}
	}

	worldSizeBlocks = WORLD_SIZE * Chunk::getChunkSize();

	//loop through all x and z
	//get the height at the xz
	//start at maxheight -1, fill with air until height
	//after height, fill with dirt.

	for (int x = 0; x < worldSizeBlocks; x++) {
		for (int z = 0; z < worldSizeBlocks; z++) {
			int h = getHeightAtXZ(glm::vec2(x, z));

			//get chunk that x z is in
			int c_x, c_z;
			c_x = x / CHUNK_SIZE;
			c_z = z / CHUNK_SIZE;

			int index = getChunkAt(glm::vec3(c_x, 0, c_z));

			for (int y = 0; y < h; y++) {
				chunks[index].blocks[x % CHUNK_SIZE][y][z % CHUNK_SIZE].setType(BLOCKTYPE_DIRT);
			}
			for (int y = h-1; y < h; y++) {
				chunks[index].blocks[x % CHUNK_SIZE][y][z % CHUNK_SIZE].setType(BLOCKTYPE_GRASS);
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
	
	double freq = 2;
	int octaves = 2;
	int waterLevel = 10;

	siv::PerlinNoise noise(1234);

	double fx = worldSizeBlocks / freq, fz = worldSizeBlocks / freq;

	double val = noise.octaveNoise0_1(position.x / fx, position.y / fz, octaves) * (CHUNK_HEIGHT / 2);

	return (int) val;
}

int World::getChunkAt(glm::vec3 position)
{
	for (int i = 0; i < chunks.size(); i++) {

		if (position == chunks[i].position) {
			return i;
		}
	}

	return -1;
}
