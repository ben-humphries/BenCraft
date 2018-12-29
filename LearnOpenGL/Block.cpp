#include "Block.h"


Block::Block()
{
	this ->type = BLOCKTYPE_AIR;
	opaque = false;
}

Block::Block(BlockType type)
{
	this->type = type;

	if (type == BLOCKTYPE_AIR)
		opaque = false;
}


Block::~Block()
{
}
