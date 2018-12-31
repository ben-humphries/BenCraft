#include "World.h"

#include "PerlinNoise.hpp"

double freq = 2;
int octaves = 2;
int waterLevel = 13;

World::World()
{
	/*for (int i = 0; i < WORLD_SIZE; i++) {
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
	}*/

	for (int x = -3; x < 3; x++) {
		for (int z = -3; z < 3; z++) {
			loadChunk(glm::vec3(x, 0, z));
		}
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

void World::updateChunks(Camera & cam)
{
	glm::vec3 cameraPos = cam.getPosition();

	if (chunks.size() > MAX_CHUNKS) {
		//remove furthest chunk
	}

	glm::vec3 chunkPos(cameraPos.x / CHUNK_SIZE, 0, cameraPos.z / CHUNK_SIZE);

	if (!isChunkLoaded(chunkPos)) {
		loadChunk(chunkPos);
	}
}

int World::getHeightAtXZ(glm::vec2 position)
{

	siv::PerlinNoise noise(1234);

	double fx = 80 / freq, fz = 80 / freq;

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

void World::loadChunk(glm::vec3 position)
{
	int x_start = position.x * CHUNK_SIZE;
	int z_start = position.z * CHUNK_SIZE;

	Chunk c;
	c.setPosition(position);

	for (int x = x_start; x < x_start + CHUNK_SIZE; x++) {
		for (int z = z_start; z < z_start + CHUNK_SIZE; z++) {
			int h = getHeightAtXZ(glm::vec2(x, z));

			int abs_x = glm::abs(x);
			int abs_z = glm::abs(z);

			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				if (y < h) {
					if (y == h - 1 && y >= waterLevel) {
						c.blocks[abs_x % CHUNK_SIZE][y][abs_z % CHUNK_SIZE].setType(BLOCKTYPE_GRASS);
					}
					else {
						c.blocks[abs_x % CHUNK_SIZE][y][abs_z % CHUNK_SIZE].setType(BLOCKTYPE_DIRT);
					}
				}
				else if (y <= waterLevel) {
					c.blocks[abs_x % CHUNK_SIZE][y][abs_z % CHUNK_SIZE].setType(BLOCKTYPE_WATER);
				}
			}


		}
	}

	c.generateMesh();

	chunks.push_back(c);
}

bool World::isChunkLoaded(glm::vec3 position)
{
	if (getChunkAt(position) == -1) {
		return false;
	}
	else {
		return true;
	}
}

void World::unloadChunk(glm::vec3 position)
{
	chunks.erase(chunks.begin() + getChunkAt(position));
}
