#pragma once
#include "API.h"
#include <unordered_map>
#include "Event.h"
#include <functional>

class BOSSENGINE_API EventManager
{
public:
	
	void AddListener(EventId eventId, std::function<void(Event&)> const& listener)
	{
		_listeners[eventId].push_back(listener);
	}

	void SendEvent(Event& event)
	{
		EventId type = event.GetType();

		for (std::function<void(Event&)> const& listener : _listeners[type])
		{
			listener(event);
		}
	}

	void SendEvent(EventId eventId)
	{
		Event event(eventId);
		
		for (std::function<void(Event&)> const& listener : _listeners[eventId])
		{
			listener(event);
		}
	}
	
private:
	std::unordered_map<EventId, std::list<std::function<void(Event&)>>> _listeners;
};
