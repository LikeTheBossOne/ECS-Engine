#pragma once
#include "API.h"
#include <cassert>
#include <vcruntime_typeinfo.h>
#include <unordered_map>
#include "ComponentArray.h"
#include <memory>

class BOSSENGINE_API ComponentManager
{
	
private:
	std::unordered_map<const char*, ComponentType> _componentTypes;
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> _componentArrays{};
	ComponentType _nextComponentType{};
	
public:

	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(_componentTypes.find(typeName) == _componentTypes.end() && "Registering component type more than once.");

		// Add this component type to the component type map
		_componentTypes.insert({ typeName, _nextComponentType });

		// Create a ComponentArray pointer and add it to the component arrays map
		_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		// Increment the value so that the next component registered will be different
		++_nextComponentType;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

		return _componentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : _componentArrays)
		{
			auto const& component = pair.second;

			component.get()->EntityDestroyed(entity);
			
			// component->EntityDestroyed(entity);
		}
	}

private:
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);
	}
};

