#pragma once

typedef enum {
	BLOCKTYPE_AIR,
	BLOCKTYPE_SLIME
} BlockType;


class Block
{
public:


	float x, y, z;

	bool opaque = true;

	BlockType type;

	Block();
	Block(BlockType type);
	~Block();
};

