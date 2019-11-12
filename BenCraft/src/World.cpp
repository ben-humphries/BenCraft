#include "World.h"



World::World()
{
	chunks = std::map<glm::ivec3, Chunk *, Vec3Compare>();
}


World::~World()
{
}

blocktype World::get(int x, int y, int z)
{
	int cx = x / CHUNK_X;
	int cy = y / CHUNK_Y;
	int cz = z / CHUNK_Z;

	int bx = x % CHUNK_X;
	int by = y % CHUNK_Y;
	int bz = z % CHUNK_Z;

	if (chunks.find(glm::ivec3(cx, cy, cz)) == chunks.end()) { //if no chunk at that chunk position exists
		return 0;
	}
	else {
		return chunks[glm::ivec3(cx, cy, cz)]->get(bx, by, bz);
	}
}

void World::set(int x, int y, int z, blocktype type)
{
	int cx = x / CHUNK_X;
	int cy = y / CHUNK_Y;
	int cz = z / CHUNK_Z;

	int bx = x % CHUNK_X;
	int by = y % CHUNK_Y;
	int bz = z % CHUNK_Z;

	//if negative, invert it so that block values are always positive, and shift chunk by 1 to account for flipped coordinates
	if (bx < 0) {
		bx = CHUNK_X + bx;
		cx -= 1;
	}
	if (by < 0) {
		by = CHUNK_Y + by;
		cy -= 1;
	}
	if (bz < 0) {
		bz = CHUNK_Z + bz;
		cz -= 1;
	}

	if (chunks.find(glm::ivec3(cx, cy, cz)) == chunks.end()) {
		chunks[glm::ivec3(cx, cy, cz)] = new Chunk();
	}

	chunks[glm::ivec3(cx, cy, cz)]->set(bx, by, bz, type);
}

void World::render(Camera * camera)
{
	std::map<glm::ivec3, Chunk *, Vec3Compare>::iterator it;
	for (it = chunks.begin(); it != chunks.end(); it++) {
		it->second->setModelMatrix(glm::translate(glm::mat4(1), glm::vec3(it->first.x * CHUNK_X, it->first.y * CHUNK_Y, it->first.z * CHUNK_Z)));
		it->second->render(camera);
	}
}