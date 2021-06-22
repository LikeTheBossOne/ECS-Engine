#pragma once
#include <glm/vec3.hpp>

#include "ChunkComponent.h"

struct World
{
	// Max amount of chunks per axis
	unsigned int chunksSize;

	// chunksSize ^ 2
	ChunkComponent** chunks;
	
	// the bottom left corner of the currently rendered map
	glm::vec3 chunk_origin;
};
