#include "Block.h"


Block::Block()
{
	this ->type = BLOCKTYPE_AIR;
	opaque = false;
}

Block::Block(BlockType type)
{
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


Block::~Block()
{
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


