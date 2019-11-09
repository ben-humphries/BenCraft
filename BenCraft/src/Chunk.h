#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Shader.h"
#include "Camera.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 16

typedef uint8_t blocktype;
typedef glm::tvec4<uint8_t> byte4; //This limits the maximum chunk size in any direction to 256, as only integers -128..127 can be represented with 8 bits

class Chunk
{
public:
	Chunk();
	~Chunk();

	blocktype get(int x, int y, int z);
	void set(int x, int y, int z, blocktype type);

	void update();
	void render(Camera * camera);

private:
	blocktype blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

	//Class-wide
	unsigned int vao;
	bool changed;

	//This vbo is specific to each mesh. So if later we add a water mesh, we will need another vbo
	unsigned int vbo;
	int numVertices;
	Shader shader;
	//
};

