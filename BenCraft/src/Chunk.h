#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Shader.h"
#include "Camera.h"

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 64

#define TEXTURE_ATLAS_SIZE 10

typedef uint8_t blocktype;
typedef glm::tvec3<uint8_t> byte3; //This limits the maximum chunk size in any direction to 256, as only integers -128..127 can be represented with 8 bits
								   //This also may only actually be 128 if the hardware/compiler doesn't support unsigned ints
typedef glm::tvec2<float> float2;

class Chunk
{
public:
	Chunk();
	~Chunk();

	blocktype get(int x, int y, int z);
	void set(int x, int y, int z, blocktype type);

	void update();
	void render(Camera * camera);

	void setModelMatrix(glm::mat4 model_matrix);

private:
	blocktype * blocks;

	glm::mat4 model_matrix;

	//Class-wide
	unsigned int vao;
	bool changed;

	//This vbo is specific to each mesh. So if later we add a water mesh, we will need another vbo
	unsigned int vbo;
	unsigned int tex_vbo;
	int numVertices;
	Shader shader;
	//
};

