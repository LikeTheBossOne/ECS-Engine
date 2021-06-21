#pragma once
#include "System.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Macros.h"
#include "BossEngine.h"

class BOSSENGINE_API PhysicsSystem : public System
{
	USYSTEM(PhysicsSystem, Transform, RigidBody)
public:
	void Init() override
	{
	}

	void Update(float dt)
	{
		for (auto const& entity : _entities)
		{
			auto& rigidBody = ECS::GetComponent<RigidBody>(entity);
			auto& transform = ECS::GetComponent<Transform>(entity);

			// Forces
			transform.position_x += rigidBody.velocity_x * dt;
			transform.position_y += rigidBody.velocity_y * dt;
			transform.position_z += rigidBody.velocity_z * dt;

			//std::cout << entity << ": " << transform.position_x << ", " << transform.position_y << ", " << transform.position_z << std::endl;
		}
	}
};
