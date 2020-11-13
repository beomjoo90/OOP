#pragma once
#include "Component.h"

class GameObject;

class PanelScript : public Component {
	int count;
public:
	PanelScript(GameObject* gameObject);
	void start() override;
	void update() override;
};

