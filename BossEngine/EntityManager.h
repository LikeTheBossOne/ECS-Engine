#pragma once
#include "API.h"
#include <queue>
#include <array>
#include <cassert>

class ECS_API EntityManager
{
private:
	std::queue<Entity> _availableEntities{};
	std::array<Signature, MAX_ENTITIES> _signatures{};
	uint32_t _livingEntityCount{};
	
public:
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			_availableEntities.push(entity);
		}
	}

	Entity CreateEntity()
	{
		assert(_livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

		Entity id = _availableEntities.front();
		_availableEntities.pop();
		++_livingEntityCount;

		return id;
	}

	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		_signatures[entity].reset();
		_availableEntities.push(entity);
		--_livingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		_signatures[entity] = signature;
	}

	Signature GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return _signatures[entity];
	}
};
