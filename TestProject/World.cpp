#include "World.h"

#include <glad/glad.h>

#include "ChunkComponent.h"
//#include "ECS.h"

World::World(unsigned chunkSize) : _chunksSize(chunkSize)
{
}

void World::SetCenter(glm::ivec3 absolutePos)
{
	glm::ivec3 offset = BlockPosToOffset(absolutePos);
	glm::ivec3 origin = offset - glm::ivec3(_chunksSize / 2, 0, _chunksSize / 2);

	if (origin == _chunkOrigin)
	{
		return;
	}

	_centerChunkOffset = offset;
	_chunkOrigin = origin;

	size_t totalChunks = _chunksSize * _chunksSize;
	ChunkComponent** oldChunks = static_cast<ChunkComponent**>(malloc(totalChunks * sizeof(ChunkComponent*)));
	memcpy(oldChunks, _chunks, totalChunks * sizeof(ChunkComponent*));

	memset(_chunks, 0, totalChunks * sizeof(ChunkComponent*));

	// Place old chunks in positions of new chunk array, destroy if they are out of bounds now
	for (unsigned int i = 0; i < totalChunks; i++) {
		struct ChunkComponent* chunk = oldChunks[i];
		if (chunk == NULL) {
			continue;
		}
		else if (ChunkInRenderDist(glm::ivec3(chunk->indexX, 0, chunk->indexZ))) {
			_chunks[GetIndexForChunk(glm::ivec3(chunk->indexX, 0, chunk->indexZ))] = chunk;
		}
		else {
			//chunk_destroy(c);
			free(chunk);
		}
	}

	
}

void World::LoadEmptyChunks()
{
	for (unsigned int i = 0; i < (_chunksSize * _chunksSize); i++) {
		// TODO: rate limit this per-frame
		if (_chunks[i] == NULL) {
			//LoadChunk(world_chunk_offset(self, i));
		}
	}
}

void World::LoadChunk(glm::ivec3 offset)
{
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	ChunkComponent* chunk = new ChunkComponent
	{
		offset.x,
		offset.z,
		_shader,
		_textureID,
		new ChunkMesh
		{
			0,
			0,
			0,
			VAO,
			VBO,
			EBO,
			buffers.data,
			buffers.indices
		},
		_defaultData
	};
	//Entity e = ECS::CreateEntity();
	//ECS::AddComponent(e, chunk);
	_chunks[GetIndexForChunk(offset)] = chunk;
}

bool World::GetBlockData(int blockPos)
{
	return true;
}

unsigned int World::GetIndexForChunk(glm::ivec3 offset)
{
	glm::ivec3 chunkPos = offset - _chunkOrigin;
	return chunkPos.z * _chunksSize + chunkPos.x;
}

bool World::ChunkInRenderDist(glm::ivec3 offset)
{
	glm::ivec3 chunkPos = offset - _chunkOrigin;
	return chunkPos.x >= 0 && chunkPos.z >= 0 && 
		chunkPos.x < static_cast<int>(_chunksSize)&& chunkPos.z < static_cast<int>(_chunksSize);
}

glm::ivec3 World::BlockPosToOffset(glm::ivec3 absolutePos)
{
	return glm::ivec3(floorf(absolutePos.x / static_cast<float>(CHUNK_SIZE)), 0, floorf(absolutePos.z / static_cast<float>(CHUNK_SIZE)));
}
