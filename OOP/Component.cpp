#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

Component::Component(GameObject* gameObject)
	: gameObject(gameObject)
{	
}

Transform* Component::getTransform() const { return gameObject->transform; }

InputManager& Component::getInputManager() const { return gameObject->inputManager; }
