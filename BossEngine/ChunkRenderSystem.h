#pragma once
/*#include <glm/vec3.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "ChunkComponent.h"
#include "Macros.h"
#include "System.h"
#include "Camera.h"
#include "Direction.h"
#include "GraphicsManager.h"
#include "Shader.h"*/

//class BOSSENGINE_API ChunkRenderSystem : public System
//{
//	USYSTEM(ChunkRenderSystem, ChunkComponent)
//public:
//	void Init() override
//	{
//	}
//
//	unsigned int PositionToIndex(unsigned int posX, unsigned int posY, unsigned int posZ)
//	{
//		return posX * CHUNK_HEIGHT * CHUNK_SIZE + posY * CHUNK_SIZE + posZ;
//	}
//
//	bool ChunkInBounds(glm::vec3 pos)
//	{
//		return pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
//			pos.x <= CHUNK_SIZE && pos.y <= CHUNK_HEIGHT && pos.z <= CHUNK_SIZE;
//	}
//
//	void Render(glm::vec3 position, glm::mat4 projection, Shader* shader, std::shared_ptr<Camera> camera,
//		unsigned textureID, unsigned VAO)
//	{
//		shader->Use();
//		shader->UniSetMat4f("view", camera->GetViewMatrix());
//		shader->UniSetMat4f("projection", projection);
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glBindVertexArray(VAO);
//
//		glm::mat4 model = glm::mat4(1.f);
//		model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
//
//		shader->UniSetMat4f("model", model);
//
//		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//	}
//
//	void Update(float dt)
//	{
//		std::shared_ptr<Camera> camera = GraphicsManager::GetMainCamera();
//		glm::vec3 camPos = camera->_position;
//		glm::mat4 projection = glm::perspective(glm::radians(camera->_fov), 800.0f / 600.0f, 0.1f, 100.0f);
//
//		for (auto const& entity : _entities)
//		{
//			auto chunk = ECS::GetComponent<ChunkComponent>(entity);
//
//			// loop over blocks in chunk
//			for (unsigned x = 0; x < CHUNK_SIZE; x++)
//			{
//				for (unsigned y = 0; y < CHUNK_HEIGHT; y++)
//				{
//					for (unsigned z = 0; z < CHUNK_SIZE; z++)
//					{
//						glm::vec3 pos(x, y, z);
//						glm::vec3 wPos = pos + camPos;
//						unsigned int data = chunk.data[PositionToIndex(x, y, z)];
//
//						if (data != 0)
//						{
//							// loop over directions
//							for (int d = 0; d < 6; d++)
//							{
//								glm::vec3 dirVec = DIRECTION_VEC[d];
//								glm::vec3 neighbor = camPos + dirVec;
//								glm::vec3 wnieghbor = wPos + dirVec;
//
//								bool visible = false;
//
//								if (ChunkInBounds(neighbor))
//								{
//									// determine if block is transparent
//									visible = true;
//								}
//
//								if (visible)
//								{
//									Render(pos, projection, chunk.shader, camera, chunk.textureID, chunk.VAO);
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//};
