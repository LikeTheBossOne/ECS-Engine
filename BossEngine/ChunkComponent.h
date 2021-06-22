#pragma once

class Shader;
const unsigned int CHUNK_SIZE = 16;
const unsigned int CHUNK_HEIGHT = 64;
const unsigned int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE;

struct ChunkComponent
{
	unsigned int positionX;
	unsigned int positionZ;
	Shader* shader;
	unsigned int textureID;
	unsigned int VAO;
	unsigned int* data; // this should be dynamically allocated
}; 