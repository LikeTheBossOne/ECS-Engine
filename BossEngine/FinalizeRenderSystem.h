#pragma once

#include "Macros.h"
#include "System.h"
#include "GraphicsManager.h"

class BOSSENGINE_API FinalizeRenderSystem : public System
{
	USYSTEM(FinalizeRenderSystem)
public:
	void Init() override
	{
	}

	void Update(float dt)
	{
		glfwSwapBuffers(GraphicsManager::GetWindow());
		glfwPollEvents();
	}
};
