#pragma once
#include "API.h"
#include "EventManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"


class BOSSENGINE_API ECS
{
private:
	static std::unique_ptr<ComponentManager> _componentManager;
	static std::unique_ptr<EntityManager> _entityManager;
	static std::unique_ptr<EventManager> _eventManager;
	static std::unique_ptr<SystemManager> _systemManager;

public:
	
public:
	
	// Entity methods
	static Entity CreateEntity()
	{
		return _entityManager->CreateEntity();
	}

	static void DestroyEntity(Entity entity)
	{	
		_entityManager->DestroyEntity(entity);

		_componentManager->EntityDestroyed(entity);

		_systemManager->EntityDestroyed(entity);
	}


	// Component methods
	template<typename T>
	static void RegisterComponent()
	{
		_componentManager->RegisterComponent<T>();
	}

	template<typename T>
	static void AddComponent(Entity entity, T component)
	{
		_componentManager->AddComponent<T>(entity, component);

		auto signature = _entityManager->GetSignature(entity);
		signature.set(_componentManager->GetComponentType<T>(), true);
		_entityManager->SetSignature(entity, signature);

		_systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	static void RemoveComponent(Entity entity)
	{
		_componentManager->RemoveComponent<T>(entity);

		auto signature = _entityManager->GetSignature(entity);
		signature.set(_componentManager->GetComponentType<T>(), false);
		_entityManager->SetSignature(entity, signature);

		_systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	static T& GetComponent(Entity entity)
	{
		return _componentManager->GetComponent<T>(entity);
	}

	template<typename T>
	static ComponentType GetComponentType()
	{
		return _componentManager->GetComponentType<T>();
	}


	// System methods
	template<typename T>
	static std::shared_ptr<T> RegisterSystem()
	{
		return _systemManager->RegisterSystem<T>();
	}

	template<typename T>
	static void SetSystemSignature(Signature signature)
	{
		_systemManager->SetSignature<T>(signature);
	}


	// Event methods
	static void AddEventListener(EventId eventId, std::function<void(Event&)> const& listener)
	{
		_eventManager->AddListener(eventId, listener);
	}

	static void SendEvent(Event& event)
	{
		_eventManager->SendEvent(event);
	}

	static void SendEvent(EventId eventId)
	{
		_eventManager->SendEvent(eventId);
	}
};

std::unique_ptr<ComponentManager> ECS::_componentManager = std::make_unique<ComponentManager>();
std::unique_ptr<EntityManager> ECS::_entityManager = std::make_unique<EntityManager>();
std::unique_ptr<SystemManager> ECS::_systemManager = std::make_unique<SystemManager>();
std::unique_ptr<EventManager> ECS::_eventManager = std::make_unique<EventManager>();
