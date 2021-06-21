#pragma once
#include "API.h"
#include <set>

class BOSSENGINE_API System
{
	virtual void Init() = 0;
public:
	virtual ~System() = default;
	
	std::set<Entity> _entities;
};
