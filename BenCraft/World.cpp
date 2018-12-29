#include "World.h"



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

	for (int i = 0; i < chunks.size(); i++) {
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
