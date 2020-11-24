#pragma once
class GameObject;
class Transform;
class InputManager;

class Component
{
protected:

	GameObject* gameObject;
	
public:
	Component(GameObject* gameObject);

	Transform* getTransform() const;

	InputManager& getInputManager() const;

	virtual void start() {}
	virtual void update() {}
	virtual void draw() {}
};

