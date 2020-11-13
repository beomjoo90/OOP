#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Screen.h"
#include "Scene.h"
#include "InputManager.h"


void GameObject::internalUpdate() {
	for (auto component : components) component->update();
	for (auto child : children) child->internalUpdate();
}

void GameObject::internalDraw() {
	for (auto component : components) component->draw();
	for (auto child : children) child->internalDraw();
}


GameObject::GameObject(const string& name, 
	const string& tag,
	GameObject* parent)
	: name(name), tag(tag), parent(parent),
	hideFlag(false), 
	transform(new Transform(this)),
	screen(Screen::getInstance()),
	scene(Scene::getInstance()),
	inputManager(InputManager::getInstance())
{	
	addComponent(transform);
}

GameObject::~GameObject() {}

void GameObject::addComponent(Component* comp) {
	if (!comp) return;
	components.push_back(comp);
}

