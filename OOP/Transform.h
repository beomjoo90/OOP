#pragma once
#include <iostream>
#include "Component.h"
#include "Utils.h"

class GameObject;


class Transform :
	public Component
{
	int count;
public:
	Transform(GameObject* gameObject)
		: Component(gameObject), count(0)
	{}

	void update() override;
};

