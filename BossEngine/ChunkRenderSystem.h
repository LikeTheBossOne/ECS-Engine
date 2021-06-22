#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "ChunkComponent.h"
#include "Macros.h"
#include "System.h"
#include "Camera.h"
#include "Enums.h"
#include "GraphicsManager.h"
#include "Shader.h"
#include "ChunkMesh.h"

class BOSSENGINE_API ChunkRenderSystem : public System
{
	USYSTEM(ChunkRenderSystem, ChunkComponent)
public:
	void Init() override
	{
	}

	bool HasOnlyBlocksAdjacent(unsigned int* data, unsigned int x, unsigned int y, unsigned int z)
	{
		if (x == 0 || y == 0 || z == 0 || x == CHUNK_SIZE - 1 || y == CHUNK_HEIGHT - 1 || z == CHUNK_SIZE - 1)
		{
			return false;
		}

		if (data[PositionToIndex(x - 1, y, z)] == 0)
		{
			return false;
		}
		if (data[PositionToIndex(x, y - 1, z)] == 0)
		{
			return false;
		}
		if (data[PositionToIndex(x, y, z - 1)] == 0)
		{
			return false;
		}
		if (data[PositionToIndex(x + 1, y, z)] == 0)
		{
			return false;
		}
		if (data[PositionToIndex(x, y + 1, z)] == 0)
		{
			return false;
		}
		if (data[PositionToIndex(x, y, z + 1)] == 0)
		{
			return false;
		}
		return true;
	}

	unsigned int PositionToIndex(unsigned int posX, unsigned int posY, unsigned int posZ)
	{
		return posX * CHUNK_HEIGHT * CHUNK_SIZE + posY * CHUNK_SIZE + posZ;
	}

	bool ChunkInBounds(glm::vec3 pos)
	{
		return pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
			pos.x < CHUNK_SIZE && pos.y < CHUNK_HEIGHT && pos.z < CHUNK_SIZE;
	}

	void Render(glm::vec3 position, glm::mat4 projection, Shader* shader, std::shared_ptr<Camera> camera,
		unsigned textureID, unsigned VAO)
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));

		shader->UniSetMat4f("model", model);
		
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	void RenderMesh(ChunkMesh* mesh, glm::vec3 chunkPosition, Shader* shader)
	{
		shader->Use();
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, chunkPosition);

		shader->UniSetMat4f("model", model);

		glBindVertexArray(mesh->VAO);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
		glDrawElements(GL_TRIANGLES, mesh->indicesIndex, GL_UNSIGNED_INT, 0);

		
		glBindVertexArray(0);
	}

	void AddFaceToMesh(ChunkMesh* mesh, glm::vec3 blockPos, Direction direction)
	{
		for (int i = 0; i < 4; i++)
		{
			const float* vertex = &CUBE_VERTICES[CUBE_INDICES[(direction * 6) + UNIQUE_INDICES[i]] * 3];			
			mesh->dataBuffer[mesh->dataIndex++] = blockPos.x + vertex[0];
			mesh->dataBuffer[mesh->dataIndex++] = blockPos.y + vertex[1];
			mesh->dataBuffer[mesh->dataIndex++] = blockPos.z + vertex[2];
			mesh->dataBuffer[mesh->dataIndex++] = CUBE_UVS[(i * 2) + 0];
			mesh->dataBuffer[mesh->dataIndex++] = CUBE_UVS[(i * 2) + 1];
		}

		for (int i = 0; i < 6; i++)
		{
			mesh->indexBuffer[mesh->indicesIndex++] = mesh->vertexCount + FACE_INDICES[i];
		}
		
		mesh->vertexCount += 4;
	}

	void BufferMesh(ChunkMesh* mesh)
	{	
		glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, mesh->dataIndex * sizeof(float), mesh->dataBuffer, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indicesIndex * sizeof(unsigned int), mesh->indexBuffer, GL_STATIC_DRAW);
	}

	void Update(float dt)
	{
		std::shared_ptr<Camera> camera = GraphicsManager::GetMainCamera();
		glm::vec3 camPos = camera->_position;
		glm::mat4 projection = glm::perspective(glm::radians(camera->_fov), 800.0f / 600.0f, 0.1f, 100.0f);

		for (auto const& entity : _entities)
		{
			auto& chunk = ECS::GetComponent<ChunkComponent>(entity);

			if (chunk.dirty)
			{
				// First reset mesh data.
				chunk.chunkMesh->vertexCount = 0;
				chunk.chunkMesh->dataIndex = 0;
				chunk.chunkMesh->indicesIndex = 0;
				chunk.chunkMesh->dataBuffer = buffers.data;
				chunk.chunkMesh->indexBuffer = buffers.indices;
			}
			
			chunk.shader->Use();
			chunk.shader->UniSetMat4f("view", camera->GetViewMatrix());
			chunk.shader->UniSetMat4f("projection", projection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, chunk.textureID);

			if (chunk.dirty)
			{
				// loop over blocks in chunk
				for (unsigned x = 0; x < CHUNK_SIZE; x++)
				{
					for (unsigned y = 0; y < CHUNK_HEIGHT; y++)
					{
						for (unsigned z = 0; z < CHUNK_SIZE; z++)
						{
							glm::ivec3 pos(x, y, z);
							//glm::vec3 wPos = pos + camPos;
							unsigned int data = chunk.data[PositionToIndex(x, y, z)];

							if (data != 0)
							{
								// loop over directions
								for (int d = 0; d < 6; d++)
								{
									glm::ivec3 dirVec = DIRECTION_VEC[d];
									glm::ivec3 neighbor = pos + dirVec;
									//glm::vec3 wnieghbor = wPos + dirVec;

									bool visible = false;

									if (ChunkInBounds(neighbor))
									{
										// determine if block is transparent (0 = transparent block)
										visible = (chunk.data[PositionToIndex(neighbor.x, neighbor.y, neighbor.z)] == 0);
									}
									else
									{
										visible = true;
									}

									if (visible)
									{
										AddFaceToMesh(chunk.chunkMesh, pos, static_cast<Direction>(d));
									}
								}
								//Render(pos, projection, chunk.shader, camera, chunk.textureID, chunk.VAO);
							}
						}
					}
				}
				chunk.dirty = 0;
			}

			BufferMesh(chunk.chunkMesh);
			RenderMesh(chunk.chunkMesh, glm::vec3(chunk.indexX * CHUNK_SIZE, 0, chunk.indexY * CHUNK_SIZE), chunk.shader);
		}
	}
};
