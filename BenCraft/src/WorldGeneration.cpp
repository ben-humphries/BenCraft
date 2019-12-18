#include "World.h"
#include "PerlinNoise.hpp"

int getHeightAtXZ(glm::vec2 position, int world_height)
{

	double freq = 2;
	int octaves = 2;

	siv::PerlinNoise noise(1234);

	double fx = 80 / freq, fz = 80 / freq;

	double val = noise.octaveNoise0_1(position.x / fx, position.y / fz, octaves) * (world_height / 2);

	return (int)val;
}


void World::loadChunk(int cx, int cy, int cz)
{

	if (chunks.find(glm::ivec3(cx, cy, cz)) != chunks.end()) { //if the chunk is already loaded
		return;
	}

	int start_block_x = cx * CHUNK_X;
	int start_block_y = cy * CHUNK_Y;
	int start_block_z = cz * CHUNK_Z;


	/*for (int x = start_block_x; x < start_block_x + CHUNK_X; x++) {
		for (int y = start_block_y; y < start_block_y + CHUNK_Y; y++) {
			for (int z = start_block_z; z < start_block_z + CHUNK_Z; z++) {
				set(x, y, z, 1);
			}
		}
	}*/

	int water_level = 13;

	for (int x = start_block_x; x < start_block_x + CHUNK_X; x++) {
		for (int z = start_block_z; z < start_block_z + CHUNK_Z; z++) {
			int h = getHeightAtXZ(glm::vec2(x, z), CHUNK_Y* 4);

			for (int y = start_block_y; y < start_block_y + CHUNK_Y; y++) {
				if (y < h) {
					if (y == h - 1) {
						if (y < water_level)
							set(x, y, z, 2); //dirt
						else
							set(x, y, z, 1); //grass
					}
					else if (y < h - 1 && y > h - 6) {
						set(x, y, z, 2); //dirt
					}
					else {
						set(x, y, z, 3); //stone
					}
				}
				else if (y <= water_level) {
					set(x, y, z, 4); //water
				}
			}
		}
	}

}