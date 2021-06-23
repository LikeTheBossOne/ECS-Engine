#pragma once
#include "Macros.h"
#include "Camera.h"
#include "ECS.h"
#include "GraphicsManager.h"
#include "RigidBody.h"
#include "System.h"

class BOSSENGINE_API PlayerMovementSystem : public System
{
	USYSTEM(PlayerMovementSystem, Player, RigidBody)
public:
	const float DefaultVelocity = 2.5f;
	
	void Init() override
	{
	}

	void Update(float dt)
	{
		GLFWwindow* window = GraphicsManager::GetWindow();
		std::shared_ptr<Camera> camera = GraphicsManager::GetMainCamera();
		
		for (auto const& entity : _entities)
		{
			auto& rigidBody = ECS::GetComponent<RigidBody>(entity);
			
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			{
				rigidBody.velocity_z = DefaultVelocity;
				camera->ProcessKeyBoard(Camera_Movement::FORWARD, dt, DefaultVelocity);
			}
				
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			{
				rigidBody.velocity_z = -DefaultVelocity;
				camera->ProcessKeyBoard(Camera_Movement::BACKWARD, dt, DefaultVelocity);
			}
				
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			{
				rigidBody.velocity_x = -DefaultVelocity;
				camera->ProcessKeyBoard(Camera_Movement::LEFT, dt, DefaultVelocity);
			}

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			{
				rigidBody.velocity_x = DefaultVelocity;
				camera->ProcessKeyBoard(Camera_Movement::RIGHT, dt, DefaultVelocity);
			}
		}
	}
};
