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
		textureOffsets.topFace = 1;
		textureOffsets.bottomFace = 2;

		textureOffsets.frontFace,
			textureOffsets.backFace,
			textureOffsets.leftFace,
			textureOffsets.rightFace
			= 0;
	}
}


Block::~Block()
{
}
