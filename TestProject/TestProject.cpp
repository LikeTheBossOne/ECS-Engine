// TestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../BossEngine/ECS.h"
#include "../BossEngine/Player.h"
#include "../BossEngine/Transform.h"
#include "../BossEngine/RigidBody.h"
#include "../BossEngine/PhysicsSystem.h"
#include "../BossEngine/PlayerControlSystem.h"
#include <random>
#include <chrono>


int main()
{
	// Initialize Components
	
    ECS::RegisterComponent<Player>();
    ECS::RegisterComponent<Transform>();
    ECS::RegisterComponent<RigidBody>();


	// Initialize Systems
	
	auto physicsSystem = ECS::RegisterSystem<PhysicsSystem>();
	physicsSystem->__INIT_SYSTEM();

	auto playerControlSystem = ECS::RegisterSystem<PlayerControlSystem>();
	playerControlSystem->__INIT_SYSTEM();


	// Setup World

	std::vector<Entity> entities(20);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> randScale(3.0f, 5.0f);

	float scale = randScale(generator);
	
	for (auto& entity : entities)
	{
		entity = ECS::CreateEntity();
		ECS::AddComponent(entity, Player{});

		ECS::AddComponent(
			entity,
			RigidBody{
				0.0f,
				0.0f,
				0.0f,
				0.0f,
				0.0f,
				0.0f
			});

		ECS::AddComponent(
			entity,
			Transform{
				randPosition(generator),
				randPosition(generator),
				randPosition(generator),
				randRotation(generator),
				randRotation(generator),
				randRotation(generator),
				scale,
				scale,
				scale
			});
	}



	// Game Loop
	
	float dt = 0.0f;

	bool quit = false;

	while (!quit)
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		playerControlSystem->Update(dt);
		physicsSystem->Update(dt);

		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
	}

	
    std::cout << "Hello World!\n";
    return 0;
}