#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

Component::Component(GameObject* gameObject)
	: gameObject(gameObject)
{
	gameObject->addComponent(this);
}
