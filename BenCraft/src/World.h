#pragma once

#include <map>

#include "Chunk.h"

struct Vec3Compare {
	/*bool operator() (const glm::vec3& lhs, const glm::vec3& rhs) const
	{
		//eventually return distance to player, but for now, distance to 0.
		return lhs.x*lhs.x + lhs.y*lhs.y + lhs.z*lhs.z < rhs.x*rhs.x + rhs.y*rhs.y + rhs.z*rhs.z;
	}*/
	bool operator () (const glm::ivec3& lhs, const glm::ivec3& rhs) const
	{
		return lhs.x < rhs.x ||
			lhs.x == rhs.x && (lhs.y < rhs.y || lhs.y == rhs.y && lhs.z < rhs.z);
	}
};

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

