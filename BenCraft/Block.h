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

	bool opaque = true;

	BlockType type;
	TextureOffsets textureOffsets;

	Block();
	Block(BlockType type);
	~Block();

	void setType(BlockType type);

private:
	void setAllFaces(int textureOffset);
};

