#include "Chunk.h"

Chunk::Chunk(int cx, int cy, int cz)
{
	blocks = new blocktype[CHUNK_X * CHUNK_Y * CHUNK_Z];

	for (int i = 0; i < CHUNK_X * CHUNK_Y * CHUNK_Z; i++) {
		blocks[i] = 0;
	}

	model_matrix = glm::mat4(1);

	glGenVertexArrays(1, &vao);
	
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &tex_vbo);

	numVertices = 0;

	changed = true;

	shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");

	this->chunkPos.x = cx;
	this->chunkPos.y = cy;
	this->chunkPos.z = cz;

	model_matrix = glm::translate(glm::mat4(1), glm::vec3(chunkPos.x * CHUNK_X, chunkPos.y * CHUNK_Y, chunkPos.z * CHUNK_Z));
}


Chunk::~Chunk()
{
	delete[] blocks;

	glBindVertexArray(vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &tex_vbo);

	glDeleteVertexArrays(1, &vao);

}

blocktype Chunk::get(int x, int y, int z)
{
	return blocks[x + CHUNK_X * (y + CHUNK_Y * z)];
}

void Chunk::set(int x, int y, int z, blocktype type)
{
	blocks[x + CHUNK_X * (y + CHUNK_Y * z)] = type;
	changed = true;
}

void Chunk::update(std::map<glm::ivec3, Chunk *, Vec3Compare> * chunks)
{
	changed = false;

	//create the mesh and assign textures
	byte3 * vertices = new byte3[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6]; //6 faces and 6 vertices per face (2 triangles)
	float2 * texCoords = new float2[CHUNK_X * CHUNK_Y * CHUNK_Z * 6 * 6 * 2];

	int i = 0;
	int j = 0;
	for (int x = 0; x < CHUNK_X; x++) {
		for (int y = 0; y < CHUNK_Y; y++) {
			for (int z = 0; z < CHUNK_Z; z++) {
				
				blocktype type = get(x, y, z);

				if (type == 0)
					continue;

				//Default front face is counter-clockwise (for face culling). 
				//So these vertices should all be added in a counter-clockwise order facing the outside of the cube.
				//Pattern is BL, BR, TL / TL, BR, TR

				//BL: 0, 1
				//BR: 1, 1
				//TL: 0, 0
				//TR: 1, 0

				//to track how many texture coords to add.
				int faces = 0;

				//Determine which block faces to draw based on adjacent blocks
				bool draw_negative_x = true;
				bool draw_positive_x = true;
				bool draw_negative_y = true;
				bool draw_positive_y = true;
				bool draw_negative_z = true;
				bool draw_positive_z = true;

				//negative x
				if (x == 0) { // chunk edge
					if (chunks->find(glm::ivec3(chunkPos.x - 1, chunkPos.y, chunkPos.z)) != chunks->end() && //if a chunk at that chunk position exists
						(*chunks)[glm::ivec3(chunkPos.x - 1, chunkPos.y, chunkPos.z)]->get(CHUNK_X - 1, y, z) != 0) { //and the adjacent block isn't air
						draw_negative_x = false;
					}
				}
				else if (get(x - 1, y, z) != 0) {
					draw_negative_x = false;
				}

				//positive x
				if (x == CHUNK_X - 1) { // chunk edge
					if (chunks->find(glm::ivec3(chunkPos.x + 1, chunkPos.y, chunkPos.z)) != chunks->end() && //if a chunk at that chunk position exists
						(*chunks)[glm::ivec3(chunkPos.x + 1, chunkPos.y, chunkPos.z)]->get(0, y, z) != 0) { //and the adjacent block isn't air
						draw_positive_x = false;
					}
				}
				else if (get(x + 1, y, z) != 0) {
					draw_positive_x = false;
				}

				//negative y
				if (y == 0) { // chunk edge
					if (chunks->find(glm::ivec3(chunkPos.x, chunkPos.y - 1, chunkPos.z)) != chunks->end() && //if a chunk at that chunk position exists
						(*chunks)[glm::ivec3(chunkPos.x, chunkPos.y - 1, chunkPos.z)]->get(x, CHUNK_Y - 1, z) != 0) { //and the adjacent block isn't air
						draw_negative_y = false;
					}
				}
				else if (get(x, y - 1, z) != 0) {
					draw_negative_y = false;
				}

				//positive y
				if (y == CHUNK_Y - 1) { // chunk edge
					if (chunks->find(glm::ivec3(chunkPos.x, chunkPos.y + 1, chunkPos.z)) != chunks->end() && //if a chunk at that chunk position exists
						(*chunks)[glm::ivec3(chunkPos.x, chunkPos.y + 1, chunkPos.z)]->get(x, 0, z) != 0) { //and the adjacent block isn't air
						draw_positive_y = false;
					}
				}
				else if (get(x, y + 1, z) != 0) {
					draw_positive_y = false;
				}

				//negative z
				if (z == 0) { // chunk edge
					if (chunks->find(glm::ivec3(chunkPos.x, chunkPos.y, chunkPos.z - 1)) != chunks->end() && //if a chunk at that chunk position exists
						(*chunks)[glm::ivec3(chunkPos.x, chunkPos.y, chunkPos.z - 1)]->get(x, y, CHUNK_Z - 1) != 0) { //and the adjacent block isn't air
						draw_negative_z = false;
					}
				}
				else if (get(x, y, z - 1) != 0) {
					draw_negative_z = false;
				}

				//positive z
				if (z == CHUNK_Z - 1) { // chunk edge
					if (chunks->find(glm::ivec3(chunkPos.x, chunkPos.y, chunkPos.z + 1)) != chunks->end() && //if a chunk at that chunk position exists
						(*chunks)[glm::ivec3(chunkPos.x, chunkPos.y, chunkPos.z + 1)]->get(x, y, 0) != 0) { //and the adjacent block isn't air
						draw_positive_z = false;
					}
				}
				else if (get(x, y, z + 1) != 0) {
					draw_positive_z = false;
				}
					

				//Add vertices to mesh, if the face should be drawn
				
				//negative x
				if (draw_negative_x) {
					vertices[i++] = byte3(x, y + 1, z);
					vertices[i++] = byte3(x, y, z);
					vertices[i++] = byte3(x, y + 1, z + 1);
					vertices[i++] = byte3(x, y + 1, z + 1);
					vertices[i++] = byte3(x, y, z);
					vertices[i++] = byte3(x, y, z + 1);

					faces++;
				}

				//positive x
				if (draw_positive_x) {
					vertices[i++] = byte3(x + 1, y, z);
					vertices[i++] = byte3(x + 1, y + 1, z);
					vertices[i++] = byte3(x + 1, y, z + 1);
					vertices[i++] = byte3(x + 1, y, z + 1);
					vertices[i++] = byte3(x + 1, y + 1, z);
					vertices[i++] = byte3(x + 1, y + 1, z + 1);

					faces++;
				}

				//negative y
				if (draw_negative_y) {
					vertices[i++] = byte3(x, y, z);
					vertices[i++] = byte3(x + 1, y, z);
					vertices[i++] = byte3(x, y, z + 1);
					vertices[i++] = byte3(x, y, z + 1);
					vertices[i++] = byte3(x + 1, y, z);
					vertices[i++] = byte3(x + 1, y, z + 1);

					faces++;
				}

				//positive y
				if (draw_positive_y) {
					vertices[i++] = byte3(x + 1, y + 1, z);
					vertices[i++] = byte3(x, y + 1, z);
					vertices[i++] = byte3(x + 1, y + 1, z + 1);
					vertices[i++] = byte3(x + 1, y + 1, z + 1);
					vertices[i++] = byte3(x, y + 1, z);
					vertices[i++] = byte3(x, y + 1, z + 1);

					faces++;
				}
				
				//negative z
				if (draw_negative_z) {
					vertices[i++] = byte3(x + 1, y + 1, z);
					vertices[i++] = byte3(x + 1, y, z);
					vertices[i++] = byte3(x, y + 1, z);
					vertices[i++] = byte3(x, y + 1, z);
					vertices[i++] = byte3(x + 1, y, z);
					vertices[i++] = byte3(x, y, z);

					faces++;
				}

				//positive z
				if (draw_positive_z) {
					vertices[i++] = byte3(x, y + 1, z + 1);
					vertices[i++] = byte3(x, y, z + 1);
					vertices[i++] = byte3(x + 1, y + 1, z + 1);
					vertices[i++] = byte3(x + 1, y + 1, z + 1);
					vertices[i++] = byte3(x, y, z + 1);
					vertices[i++] = byte3(x + 1, y, z + 1);

					faces++;
				}

				//vertex order is the same for each quad, so texture coords are the same.
				float offset_x = type % TEXTURE_ATLAS_SIZE;
				float offset_y = type / TEXTURE_ATLAS_SIZE;

				//TODO: make this such that you can have different textures for each block face.
				for (int a = 0; a < faces; a++) {
					
					texCoords[j++] = float2((0.0f + offset_x) / TEXTURE_ATLAS_SIZE, (1.0f + offset_y) / TEXTURE_ATLAS_SIZE);
					texCoords[j++] = float2((1.0f + offset_x) / TEXTURE_ATLAS_SIZE, (1.0f + offset_y) / TEXTURE_ATLAS_SIZE);
					texCoords[j++] = float2((0.0f + offset_x) / TEXTURE_ATLAS_SIZE, (0.0f + offset_y) / TEXTURE_ATLAS_SIZE);
					texCoords[j++] = float2((0.0f + offset_x) / TEXTURE_ATLAS_SIZE, (0.0f + offset_y) / TEXTURE_ATLAS_SIZE);
					texCoords[j++] = float2((1.0f + offset_x) / TEXTURE_ATLAS_SIZE, (1.0f + offset_y) / TEXTURE_ATLAS_SIZE);
					texCoords[j++] = float2((1.0f + offset_x) / TEXTURE_ATLAS_SIZE, (0.0f + offset_y) / TEXTURE_ATLAS_SIZE);
				}
			}
		}	
	}

	numVertices = i;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(*texCoords), texCoords, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(*vertices), vertices, GL_STATIC_DRAW);

	//first attrib is for vertices. 3 consecutive bytes in the array.
	glVertexAttribPointer(0, 3, GL_BYTE, GL_FALSE, 3 * sizeof(uint8_t), (void*)0);

	//second attrib is for texCoords. 2 bytes.
	glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	delete[] vertices;
	delete[] texCoords;
}

void Chunk::render(Camera * camera, std::map<glm::ivec3, Chunk *, Vec3Compare> * chunks)
{
	if (changed)
		update(chunks);

	//empty chunk
	if (numVertices == 0)
		return;

	//render the mesh
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	shader.use();

	glm::mat4 trans = camera->getProjectionMatrix() * camera->getViewMatrix() * model_matrix;
	shader.setMat4("transform", trans);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}
