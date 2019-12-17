#pragma once

#include <map>

#include "Chunk.h"
#include "Vec3Compare.h"

class World
{
public:
	World();
	~World();

	blocktype get(int x, int y, int z);
	void set(int x, int y, int z, blocktype type);
	void render(Camera * camera);


private:
	std::map<glm::ivec3, Chunk *, Vec3Compare> chunks;
};

