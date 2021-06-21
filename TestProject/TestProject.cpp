// TestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <chrono>

#include "FinalizeRenderSystem.h"
#include "PhysicsSystem.h"
#include "Player.h"
#include "PlayerMovementSystem.h"
#include "PreRenderSystem.h"
#include "Renderable.h"
#include "RenderSystem.h"
#include "RigidBody.h"
#include "load_stbi_image.h"
#include "Transform.h"
#include "Camera.h"

void setupVertices(unsigned int *VAO, unsigned int *VBO, float vertices[], int verticesSize)
{
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void LoadTexture(const char* pathToTexture, unsigned int* textureID, int rgba, int glRepeat)
{
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glRepeat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glRepeat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(pathToTexture, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgba, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

int main()
{
	// Start Engine : Initialize static classes
	BossEngine::Start();
	
	// Initialize Components
    ECS::RegisterComponent<Player>();
    ECS::RegisterComponent<Transform>();
	ECS::RegisterComponent<RigidBody>();
	ECS::RegisterComponent<Renderable>();

	// Initialize Systems
	auto physicsSystem = ECS::RegisterSystem<PhysicsSystem>();
	physicsSystem->__INIT_SYSTEM();

	auto playerMovementSystem = ECS::RegisterSystem<PlayerMovementSystem>();
	playerMovementSystem->__INIT_SYSTEM();

	auto preRenderSystem = ECS::RegisterSystem<PreRenderSystem>();
	preRenderSystem->__INIT_SYSTEM();

	auto renderSystem = ECS::RegisterSystem<RenderSystem>();
	renderSystem->__INIT_SYSTEM();

	auto finalRenderSystem = ECS::RegisterSystem<FinalizeRenderSystem>();
	finalRenderSystem->__INIT_SYSTEM();

	// Setup World
#pragma region Setup World
	Entity player = ECS::CreateEntity();
	ECS::AddComponent(player, Player{});

	ECS::AddComponent(
		player,
		RigidBody{
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f
		});

	ECS::AddComponent(
		player,
		Transform{
			0,
			0,
			-10,
			0,
			0,
			0,
			0,
			0,
			0
		});


	// Setup Renderable entities
	Entity e = ECS::CreateEntity();

	unsigned int texture;
	LoadTexture("Resources/textures/block/dirt.png", &texture, GL_RGBA, GL_CLAMP_TO_EDGE);

	unsigned int VAO;
	unsigned int VBO;
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	};

	setupVertices(&VAO, &VBO, vertices, sizeof(vertices) / sizeof(float));

	Shader* shader = new Shader(R"(Shaders\vertex1.vs)", R"(Shaders\fragment1.fs)");
	shader->Use();
	shader->UniSetInt("texture1", 0);
	
	Renderable r
	{
		texture,
		VAO,
		shader
	};
	
	ECS::AddComponent(e, r);
	ECS::AddComponent(e, 
		Transform
		{
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0,
			0
		}
	);
	
#pragma endregion


	// Game Loop
	
	float dt = 0.0f;

	while (!glfwWindowShouldClose(GraphicsManager::GetWindow()))
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		// Pre-Update
		preRenderSystem->Update(dt);
		
		// Update
		playerMovementSystem->Update(dt);
		physicsSystem->Update(dt);

		// Render
		renderSystem->Update(dt);

		// Finalize Render
		finalRenderSystem->Update(dt);
		
		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
	}

	
    std::cout << "Hello World!\n";
    return 0;
}
