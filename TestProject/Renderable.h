#pragma once
#include "Shader.h"

/*
 * A class that holds all information for rendering
 *
 * Any texture or shader in component must already be loaded.
 * Render offsets are offset starting at transform
 */
struct Renderable
{
	unsigned int textureID;
	unsigned int VAO;
	Shader* shader;
};