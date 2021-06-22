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
#include "ChunkComponent.h"
#include "ChunkRenderSystem.h"
#include "Mesh.h"

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

void setupVertices(unsigned int *VAO, unsigned int *VBO, unsigned int* EBO, float vertices[], int verticesSize, unsigned int indices[], int indicesSize)
{
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);
	glGenBuffers(1, EBO);

	glBindVertexArray(*VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void setupBuffers(unsigned int* VAO)
{
	unsigned int VBO;
	unsigned int EBO;

	float blockVertices[20 * 6];
	int vertIdx = 0;
	unsigned int blockIndices[6 * 6];
	int blockIdx = 0;
	int vertCount = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			const float* vertex = &CUBE_VERTICES[CUBE_INDICES[(i * 6) + UNIQUE_INDICES[j]] * 3];
			blockVertices[vertIdx++] = vertex[0];
			blockVertices[vertIdx++] = vertex[1];
			blockVertices[vertIdx++] = vertex[2];
			// texture
			blockVertices[vertIdx++] = CUBE_UVS[(j * 2) + 0];
			blockVertices[vertIdx++] = CUBE_UVS[(j * 2) + 1];
		}

		for (int j = 0; j < 6; j++)
		{
			blockIndices[blockIdx++] = vertCount + FACE_INDICES[j];
		}

		vertCount += 4;
	}

	setupVertices(VAO, &VBO, &EBO, blockVertices, sizeof(blockVertices), blockIndices, sizeof(blockIndices));
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
	//ECS::RegisterComponent<ChunkComponent>();

	// Initialize Systems
	auto physicsSystem = ECS::RegisterSystem<PhysicsSystem>();
	physicsSystem->__INIT_SYSTEM();

	auto playerMovementSystem = ECS::RegisterSystem<PlayerMovementSystem>();
	playerMovementSystem->__INIT_SYSTEM();

	auto preRenderSystem = ECS::RegisterSystem<PreRenderSystem>();
	preRenderSystem->__INIT_SYSTEM();

	auto renderSystem = ECS::RegisterSystem<RenderSystem>();
	renderSystem->__INIT_SYSTEM();

	//auto chunkRenderSystem = ECS::RegisterSystem<ChunkRenderSystem>();
	//chunkRenderSystem->__INIT_SYSTEM();
	
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

	unsigned int texture;
	LoadTexture("Resources/textures/block/dirt.png", &texture, GL_RGBA, GL_CLAMP_TO_EDGE);

	unsigned int VAO;
	setupBuffers(&VAO);

	Shader* shader = new Shader(R"(Shaders\vertex1.vs)", R"(Shaders\fragment1.fs)");
	shader->Use();
	shader->UniSetInt("texture1", 0);

	/*Entity e = ECS::CreateEntity();

	unsigned int* data;
	data = new unsigned int[CHUNK_VOLUME];
	for (int i = 0; i < CHUNK_VOLUME; i++)
	{
		data[i] = 1;
	}
	ECS::AddComponent(e, ChunkComponent
		{
			0,
			0,
			shader,
			texture,
			VAO,
			data
		}
	);*/
	
	/*for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 8; z++)
		{
			for (int y = 0; y < 8; y++)
			{
				Entity e = ECS::CreateEntity();

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
						(float)x,
						(float)y,
						(float)z,
						0,
						0,
						0,
						0,
						0,
						0
					}
				);
			}
		}
		
	}
	*/
	
	
#pragma endregion


	// Game Loop
	float wireframeCooldown = 0;
	
	float dt = 0.0f;
	while (!glfwWindowShouldClose(GraphicsManager::GetWindow()))
	{
		auto startTime = std::chrono::high_resolution_clock::now();


		if (glfwGetKey(GraphicsManager::GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(GraphicsManager::GetWindow(), 1);
		}
		if (glfwGetKey(GraphicsManager::GetWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS && (wireframeCooldown > 0.1f))
		{
			wireframeCooldown = 0;
			GraphicsManager::ToggleWireframe();
		}
		

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
		std::cout << dt * 1000 << std::endl;

		if (wireframeCooldown < 10)
		{
			wireframeCooldown += dt;
		}
	}

	
    std::cout << "Hello World!\n";
    return 0;
}
