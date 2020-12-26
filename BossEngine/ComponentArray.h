#pragma once
#include "API.h"
#include <array>
#include <cassert>
#include <unordered_map>

class ECS_API IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};


/**
 * For each component, there will be an array of components for each entity with that component
 */
template<typename T>
class ECS_API ComponentArray : public IComponentArray
{
private:
	std::array<T, MAX_ENTITIES> _componentArray{};
	std::unordered_map<Entity, size_t> _entityToIndexMap{};
	std::unordered_map<size_t, Entity> _indexToEntityMap{};
	size_t _size{};
	
public:
	void InsertData(Entity entity, T component)
	{
		assert(_entityToIndexMap.find(entity) == _entityToIndexMap.end() && "Component added to same entity more than once.");
		
		size_t newIndex = _size;
		_entityToIndexMap[entity] = newIndex;
		_indexToEntityMap[newIndex] = entity;
		_componentArray[newIndex] = component;
		++_size;

	}

	void RemoveData(Entity entity)
	{
		assert(_entityToIndexMap.find(entity) != _entityToIndexMap.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = _entityToIndexMap[entity];
		size_t indexOfLastElement = _size - 1;
		_componentArray[indexOfRemovedEntity] = _componentArray[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = _indexToEntityMap[indexOfLastElement];
		_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		_entityToIndexMap.erase(entity);
		_indexToEntityMap.erase(indexOfLastElement);

		--_size;
	}

	T& GetData(Entity entity)
	{
		return _componentArray[_entityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (_entityToIndexMap.find(entity) != _entityToIndexMap.end())
		{
			RemoveData(entity);
		}
	}
};