#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Screen.h"
#include "Scene.h"
#include "InputManager.h"
#include <algorithm>


map<string, vector<GameObject *>> GameObject::objects;

GameObject* GameObject::Find(const string& name)
{
	if (objects.empty()) return nullptr;

	auto found = find_if(objects.begin(), objects.end(),
		[=](auto& pair) { return pair.first == name; } );
	string path = name;
	auto idx = path.find_first_of('/');
	if (idx == string::npos) return nullptr;
	auto temporaryName = path.substr( idx + 1);

	auto found2 = find_if(objects.begin(), objects.end(),
		[=](auto& pair) { return pair->first == temporaryName;  });
	if (found2 == objects.end()) return nullptr;
	auto entry = found2->second;
	if (entry.size() == 0) return nullptr;
	auto found3 = find_if(entry.begin(), entry.end(),
		[=](auto obj) { return obj->absolutePath == path});
	if (found3 == entry.end()) return nullptr;
	return *found3;
}

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

