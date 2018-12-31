#include "World.h"

#include "PerlinNoise.hpp"

double freq = 2;
int octaves = 2;
int waterLevel = 13;

const float ROOT_TWO = 1.4142135623730950488016887242097;

World::World(Camera * cam)
{
	startUpdatingChunks(cam);
}


World::~World()
{
}

void World::render(Camera & cam)
{

	for (int i = 0; i < chunks.size(); i++) {
		if (isChunkLoaded(i)) {
			if (!chunks[i].meshesBoundToVAO) {
				chunks[i].bindMeshesToVAO();
			}

			chunks[i].renderTerrain(cam);
		}

	}

	//Sort the transparent meshes and render them last to first.
	std::map<float, int> sorted = sortChunksByDistanceToCamera(cam);

	for (std::map<float, int>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		if (isChunkLoaded(it->second)) {
			chunks[it->second].renderWater(cam);
		}
	}
}

void World::updateChunks(Camera * cam)
{

	while (true) {
		glm::vec3 cameraPos = cam->getPosition();

		int c_x;
		int c_z;

		if (cameraPos.x < 0) {
			c_x = (cameraPos.x - CHUNK_SIZE) / CHUNK_SIZE;
		}
		else {
			c_x = cameraPos.x / CHUNK_SIZE;
		}

		if (cameraPos.z < 0) {
			c_z = (cameraPos.z - CHUNK_SIZE) / CHUNK_SIZE;
		}
		else {
			c_z = cameraPos.z / CHUNK_SIZE;
		}

		glm::vec3 chunkPos(c_x, 0, c_z);

		if (chunks.size() > MAX_CHUNKS) {
			//remove furthest chunk
			std::map<float, int> sorted = sortChunksByDistanceToCamera(*cam);

			if (glm::distance(chunkPos, chunks[sorted.rbegin()->second].position) > SQRT_MAX_CHUNKS / 2 * ROOT_TWO) {
				unloadChunk(sorted.rbegin()->second);
			}


		}
		else {

			int x, y, dx, dy;
			x = y = dx = 0;
			dy = -1;
			int t = SQRT_MAX_CHUNKS;
			int maxI = t * t;
			for (int i = 0; i < maxI; i++) {
				if ((-SQRT_MAX_CHUNKS / 2 <= x) && (x <= SQRT_MAX_CHUNKS / 2) && (-SQRT_MAX_CHUNKS / 2 <= y) && (y <= SQRT_MAX_CHUNKS / 2)) {
					glm::vec3 pos(chunkPos.x + x, 0, chunkPos.z + y);

					if (!isChunkLoaded(pos)) {
						loadChunk(pos);
						break;
					}
				}
				if ((x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1 - y))) {
					t = dx;
					dx = -dy;
					dy = t;
				}
				x += dx;
				y += dy;
			}
		}
	}

}

void World::startUpdatingChunks(Camera * cam) {

	update_chunks_thread = std::thread(&World::updateChunks, this, cam);
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

			int abs_x = x - x_start;
			int abs_z = z - z_start;

			for (int y = 0; y < CHUNK_HEIGHT; y++) {
				if (y < h) {
					if (y == h - 1 && y >= waterLevel) {
						c.blocks[abs_x][y][abs_z].setType(BLOCKTYPE_GRASS);
					}
					else {
						c.blocks[abs_x][y][abs_z].setType(BLOCKTYPE_DIRT);
					}
				}
				else if (y <= waterLevel) {
					c.blocks[abs_x][y][abs_z].setType(BLOCKTYPE_WATER);
				}
			}


		}
	}

	c.generateMesh();
	c.loaded = true;
	chunks.push_back(c);
}

bool World::isChunkLoaded(glm::vec3 position)
{
	int index = getChunkAt(position);
	
	return isChunkLoaded(index);
}

bool World::isChunkLoaded(int index)
{
	if (index != -1 && chunks[index].loaded) {
		return true;
	}
	/*if (index != -1) {
		return true;
	}*/
	return false;
}

void World::unloadChunk(glm::vec3 position)
{
	chunks[getChunkAt(position)].cleanup();
	chunks.erase(chunks.begin() + getChunkAt(position));
}

void World::unloadChunk(int index)
{
	chunks[index].cleanup();
	chunks.erase(chunks.begin() + index);
}
std::map<float, int> World::sortChunksByDistanceToCamera(Camera & cam)
{
	std::map<float, int> sorted;
	for (unsigned int i = 0; i < chunks.size(); i++)
	{
		glm::vec3 b_position = glm::vec3(chunks[i].position.x * CHUNK_SIZE + CHUNK_SIZE / 2,
										chunks[i].position.y * CHUNK_HEIGHT + CHUNK_SIZE / 2,
										chunks[i].position.z * CHUNK_SIZE + CHUNK_SIZE / 2);
		float distance = glm::length(cam.getPosition() - b_position);
		sorted[distance] = i;
	}

	return sorted;
}
