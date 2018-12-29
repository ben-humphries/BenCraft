#pragma once

typedef enum {
	BLOCKTYPE_AIR,
	BLOCKTYPE_GRASS,
	BLOCKTYPE_DIRT,
	BLOCKTYPE_STONE
} BlockType;

typedef struct {
	int frontFace,
		backFace,
		leftFace,
		rightFace,
		topFace,
		bottomFace
		= 0;
} TextureOffsets;

class Block
{
public:

	float x, y, z;

	bool opaque = true;

	BlockType type;
	TextureOffsets textureOffsets;

	Block();
	Block(BlockType type);
	~Block();

private:
	void setAllFaces(int textureOffset);
};

