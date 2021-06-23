#pragma once
#include <glm/vec3.hpp>

class Shader;
struct ChunkComponent;

class World
{
public:
	// Max amount of chunks per axis
	unsigned int _chunksSize;

	// chunksSize ^ 2
	ChunkComponent** _chunks;

	// the bottom left corner of the currently rendered map
	glm::ivec3 _chunkOrigin;

	glm::ivec3 _centerChunkOffset;

	//TEMP
	Shader* _shader;
	unsigned int _textureID;

	unsigned int* _defaultData;

	
	World(unsigned int chunkSize);

	void SetCenter(glm::ivec3 absolutePos);
	

	bool GetBlockData(int blockPos);
	unsigned int GetIndexForChunk(glm::ivec3 offset);
	bool ChunkInRenderDist(glm::ivec3 offset);
	
	static glm::ivec3 BlockPosToOffset(glm::ivec3 absolutePos);

private:
	void LoadEmptyChunks();
	void LoadChunk(glm::ivec3 offset);
};
