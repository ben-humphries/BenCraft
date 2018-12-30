#include "Block.h"

#include <iostream>

Block::Block()
{
}

Block::Block(BlockType type)
{
	setType(type);
}


Block::~Block()
{
}

void Block::setType(BlockType type)
{
	opaque = true;
	this->type = type;

	if (type == BLOCKTYPE_AIR) {
		opaque = false;
	}
	else if (type == BLOCKTYPE_GRASS) {
		setAllFaces(0);

		textureOffsets.topFace = 1;
		textureOffsets.bottomFace = 2;
	}
	else if (type == BLOCKTYPE_DIRT) {
		setAllFaces(2);
	}
	else if (type == BLOCKTYPE_STONE) {
		setAllFaces(3);
	}
}

void Block::setAllFaces(int textureOffset)
{
	textureOffsets.topFace = textureOffset;
	textureOffsets.bottomFace = textureOffset;
	textureOffsets.frontFace = textureOffset;
	textureOffsets.backFace = textureOffset;
	textureOffsets.leftFace = textureOffset;
	textureOffsets.rightFace = textureOffset;

}


