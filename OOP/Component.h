#pragma once
class GameObject;
class Transform;

class Component
{
protected:

	GameObject* gameObject;
	
public:
	Component(GameObject* gameObject);

	virtual void start() {}
	virtual void update() {}
	virtual void draw() {}
};

