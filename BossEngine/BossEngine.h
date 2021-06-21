#pragma once
#include "GraphicsManager.h"

/*
 * This is the entrypoint for a game programmer. They should not interact with any engine systems other than the ones defined in this.
 */
class BOSSENGINE_API BossEngine
{
public:
	static void Start()
	{
		GraphicsManager::Start();
	}
};
