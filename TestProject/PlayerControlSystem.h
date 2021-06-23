#pragma once
#include <bitset>
#include "System.h"
#include "Event.h"
#include "ECS.h"
#include <iostream>

#include "Player.h"
#include "Transform.h"

class BOSSENGINE_API PlayerControlSystem : public System
{
	USYSTEM(PlayerControlSystem, Player, Transform)
public:
	void Init() override
	{
		//ECS.AddEventListener(METHOD_LISTENER(Events::Window::INPUT, PlayerControlSystem::InputListener));
	}

	void Update(float dt)
	{
		//std::cout << "Player Control updated";
	}

private:
	std::bitset<8> mButtons;

	void InputListener(Event& event)
	{
		
	}
};
