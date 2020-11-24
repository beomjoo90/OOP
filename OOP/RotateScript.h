#pragma once
#include "Component.h"

class RotateScript :
	public Component
{
	Transform* transform;
	char* shapeData;

	void rotate();

public:

	RotateScript(GameObject* gameObject);
	virtual ~RotateScript();

	void start() override;
	void update() override;
};

