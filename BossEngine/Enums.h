#pragma once

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

enum Direction
{
	NORTH = 0,
	SOUTH,
	EAST,
	WEST,
	UP,
	DOWN
};

const glm::vec3 DIRECTION_VEC[6] = {
	{  0,  0, -1 },
	{  0,  0,  1 },
	{  1,  0,  0 },
	{ -1,  0,  0 },
	{  0,  1,  0 },
	{  0, -1,  0 },
};