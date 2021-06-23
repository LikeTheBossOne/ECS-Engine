#pragma once
#include "System.h"
#include "Macros.h"
#include "Renderable.h"
#include "Transform.h"
#include "GraphicsManager.h"

class BOSSENGINE_API RenderSystem : public System
{
	USYSTEM(RenderSystem, Transform, Renderable)
public:
	void Init() override
	{
	}

	void Update(float dt)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto camera = GraphicsManager::GetMainCamera();
		
		glm::mat4 projection = glm::perspective(glm::radians(camera->_fov), 800.0f / 600.0f, 0.1f, 100.0f);
		
		for (auto const& entity : _entities)
		{
			auto& renderable = ECS::GetComponent<Renderable>(entity);
			auto& transform = ECS::GetComponent<Transform>(entity);
			
			Shader* shader = renderable.shader;
			shader->Use();
			shader->UniSetMat4f("view", camera->GetViewMatrix());
			shader->UniSetMat4f("projection", projection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderable.textureID);
			glBindVertexArray(renderable.VAO);

			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, glm::vec3(transform.position_x, transform.position_y, transform.position_z));
			
			shader->UniSetMat4f("model", model);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
	}
};
