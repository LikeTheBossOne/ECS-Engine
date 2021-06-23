#pragma once
#include "ChunkMesh.h"

struct World;
class Shader;
const unsigned int CHUNK_SIZE = 16;
const unsigned int CHUNK_HEIGHT = 128;
const unsigned int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE;

const int FACE_INDICES[] = { 1, 0, 3, 1, 3, 2 };
const int UNIQUE_INDICES[] = { 1, 0, 5, 2 };
const int CUBE_INDICES[] = {
	1, 0, 3, 1, 3, 2, // north (-z)
	4, 5, 6, 4, 6, 7, // south (+z)
	5, 1, 2, 5, 2, 6, // east (+x)
	0, 4, 7, 0, 7, 3, // west (-x)
	2, 3, 7, 2, 7, 6, // top (+y)
	5, 4, 0, 5, 0, 1, // bottom (-y)
};

const float CUBE_VERTICES[] = {
	0, 0, 0,
	1, 0, 0,
	1, 1, 0,
	0, 1, 0,

	0, 0, 1,
	1, 0, 1,
	1, 1, 1,
	0, 1, 1
};

const float CUBE_UVS[] = {
	1, 0,
	0, 0,
	0, 1,
	1, 1
};

// TODO: if threading is introduced, make these per-thread
inline struct
{
	float data[(CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE) * 8 * 5 * sizeof(float)];
	unsigned int indices[(CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE) * 36 * sizeof(unsigned int)];
} buffers;

struct ChunkComponent
{
	int indexX;
	int indexZ;
	Shader* shader;
	unsigned int textureID;
	ChunkMesh* chunkMesh;
	unsigned int* data; // this should be dynamically allocated

	bool dirty = true;

	World* world;
}; 