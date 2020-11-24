#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Screen.h"
#include "Scene.h"
#include "InputManager.h"

void GameObject::internalStart() {
	for (auto component : components) component->start();
	for (auto child : children) child->internalStart();
}

void GameObject::internalUpdate() {
	for (auto component : components) component->update();
	for (auto child : children) child->internalUpdate();
}


void GameObject::internalDraw() {
	for (auto component : components) component->draw();
	for (auto child : children) child->internalDraw();
}

GameObject* GameObject::Instantiate(const string& name, const string& tag, GameObject* parent, 
	const Position& pos, const string& shape, const Position& size )
{
	auto gameObject = new GameObject(name, tag, parent);
	gameObject->transform->setPosition(pos);
	gameObject->transform->setShape(shape);
	gameObject->transform->setSize(size);

	if (parent == nullptr) Scene::getInstance().add(gameObject);
	else parent->add(gameObject);

	return gameObject;
}


GameObject::GameObject(const string& name, 
	const string& tag,
	GameObject* parent)
	: name(name), tag(tag), parent(parent),
	hideFlag(false), 
	transform(nullptr),
	screen(Screen::getInstance()),
	scene(Scene::getInstance()),
	inputManager(InputManager::getInstance())
{	
	transform = getOrAddComponent<Transform>();
}

GameObject::~GameObject() {}

