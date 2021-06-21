#pragma once
#include "Macros.h"
#include "System.h"
#include <glad/glad.h>

class BOSSENGINE_API PreRenderSystem : public System
{
	USYSTEM(PreRenderSystem)
public:
	void Init() override
	{
	}

	void Update(float dt)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
