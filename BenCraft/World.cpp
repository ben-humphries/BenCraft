#include "World.h"

#include "PerlinNoise.hpp"

double freq = 2;
int octaves = 2;
int waterLevel = 13;

/*
- dont add any edge faces to chunks, but store what blocks are edge blocks in a list in each chunk
- after chunk meshes are generated (and more importantly, all blocks have been set), loop through each chunk
- check for adjacent chunks
*/

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

			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				if (y < h) {
					if (y == h - 1 && y >= waterLevel) {
						chunks[index].blocks[x % CHUNK_SIZE][y][z % CHUNK_SIZE].setType(BLOCKTYPE_GRASS);
					}
					else {
						chunks[index].blocks[x % CHUNK_SIZE][y][z % CHUNK_SIZE].setType(BLOCKTYPE_DIRT);
					}
				}
				else if (y <= waterLevel) {
					chunks[index].blocks[x % CHUNK_SIZE][y][z % CHUNK_SIZE].setType(BLOCKTYPE_WATER);
				}
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
		chunks[i].renderTerrain(cam);
	}

	//Sort the transparent meshes and render them last to first.
	std::map<float, int> sorted;
	for (unsigned int i = 0; i < chunks.size(); i++)
	{
		float distance = glm::length(cam.getPosition() - chunks[i].position);
		sorted[distance] = i;
	}

	for (std::map<float, int>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		chunks[it->second].renderWater(cam);
	}
}

int World::getHeightAtXZ(glm::vec2 position)
{

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
