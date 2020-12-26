#pragma once
#include <unordered_map>
#include "API.h"
#include <any>

class ECS_API Event
{
public:
	
	Event() = delete;

	explicit Event(EventId type)
		: _type(type)
	{}

	template<typename T>
	void SetParam(EventId id, T value)
	{
		_data[id] = value;
	}

	template<typename T>
	T GetParam(EventId id)
	{
		return std::any_cast<T>(_data[id]);
	}

	EventId GetType() const
	{
		return _type;
	}
private:
	EventId _type{};
	std::unordered_map<EventId, std::any> _data{};
};
