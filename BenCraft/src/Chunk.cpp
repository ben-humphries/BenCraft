#include "Chunk.h"

Chunk::Chunk()
{
	memset(blocks, 0, sizeof(blocks));

	glGenVertexArrays(1, &vao);
	
	glGenBuffers(1, &vbo);
	numVertices = 0;

	changed = true;

	shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

}


Chunk::~Chunk()
{
	glBindVertexArray(vao);
	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

}

blocktype Chunk::get(int x, int y, int z)
{
	return blocks[x][y][z];
}

void Chunk::set(int x, int y, int z, blocktype type)
{
	blocks[x][y][z] = type;
	changed = true;
}

void Chunk::update()
{
	changed = false;

	//create the mesh
	byte4 vertices[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6]; //6 faces and 6 vertices per face (2 triangles)

	int i = 0;
	for (int x = 0; x < CHUNK_X; x++) {
		for (int y = 0; y < CHUNK_Y; y++) {
			for (int z = 0; z < CHUNK_Z; z++) {
				
				blocktype type = blocks[x][y][z];

				if (type == 0)
					continue;

				//negative x
				vertices[i++] = byte4(x, y, z, type);
				vertices[i++] = byte4(x, y, z + 1, type);
				vertices[i++] = byte4(x, y + 1, z, type);
				vertices[i++] = byte4(x, y + 1, z, type);
				vertices[i++] = byte4(x, y, z + 1, type);
				vertices[i++] = byte4(x, y + 1, z + 1, type);

				//positive x
				vertices[i++] = byte4(x + 1, y, z, type);
				vertices[i++] = byte4(x + 1, y, z + 1, type);
				vertices[i++] = byte4(x + 1, y + 1, z, type);
				vertices[i++] = byte4(x + 1, y + 1, z, type);
				vertices[i++] = byte4(x + 1, y + 1, z + 1, type);
				vertices[i++] = byte4(x + 1, y, z + 1, type);

				//negative y
				vertices[i++] = byte4(x, y, z, type);
				vertices[i++] = byte4(x, y, z + 1, type);
				vertices[i++] = byte4(x + 1, y, z, type);
				vertices[i++] = byte4(x + 1, y, z, type);
				vertices[i++] = byte4(x, y, z + 1, type);
				vertices[i++] = byte4(x + 1, y, z + 1, type);

				//positive y
				vertices[i++] = byte4(x, y + 1, z, type);
				vertices[i++] = byte4(x, y + 1, z + 1, type);
				vertices[i++] = byte4(x + 1, y + 1, z, type);
				vertices[i++] = byte4(x + 1, y + 1, z, type);
				vertices[i++] = byte4(x, y + 1, z + 1, type);
				vertices[i++] = byte4(x + 1, y + 1, z + 1, type);

				//negative z
				vertices[i++] = byte4(x, y, z, type);
				vertices[i++] = byte4(x, y + 1, z, type);
				vertices[i++] = byte4(x + 1, y, z, type);
				vertices[i++] = byte4(x + 1, y, z, type);
				vertices[i++] = byte4(x, y + 1, z, type);
				vertices[i++] = byte4(x + 1, y + 1, z, type);

				//positive z
				vertices[i++] = byte4(x, y, z + 1, type);
				vertices[i++] = byte4(x, y + 1, z + 1, type);
				vertices[i++] = byte4(x + 1, y, z + 1, type);
				vertices[i++] = byte4(x + 1, y, z + 1, type);
				vertices[i++] = byte4(x, y + 1, z + 1, type);
				vertices[i++] = byte4(x + 1, y + 1, z + 1, type);

			}
		}	
	}

	numVertices = i;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(*vertices), vertices, GL_STATIC_DRAW);

	//first attrib is for vertices. 3 consecutive bytes in the array.
	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 4 * sizeof(uint8_t), (void*)0);
	//second attrib is for color. 1 byte.
	glVertexAttribPointer(1, 1, GL_BYTE, GL_FALSE, 4 * sizeof(uint8_t), (void*)(3 * sizeof(uint8_t)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Chunk::render(Camera * camera)
{
	if (changed)
		update();

	//empty chunk
	if (numVertices == 0)
		return;

	//render the mesh
	//Fix culling. Probably has to do with the vertex order above.
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	shader.use();

	glm::mat4 trans = camera->getProjectionMatrix() * camera->getViewMatrix() * glm::mat4(1); //model;
	shader.setMat4("transform", trans);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}
