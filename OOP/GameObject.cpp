#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Screen.h"
#include "Scene.h"
#include "InputManager.h"
#include <algorithm>

map<string, vector<GameObject *>> GameObject::objects;

void GameObject::Add(GameObject* obj)
{
	if (!obj) return;

	auto it = objects.find(obj->name);
	if (it == objects.cend())
		objects[obj->name] = vector<GameObject *>();

	auto& entry = objects[obj->name];
	auto found = find_if(entry.cbegin(), entry.cend(), 
		[&](auto item) { return item->absolutePath == obj->absolutePath; } );

	if (found != entry.cend()) return;
	entry.push_back(obj);
}

void GameObject::Remove(GameObject* obj)
{
	if (obj == nullptr) return;

	try {
		auto& entry = objects[obj->name]; // NOTE: entry should not be copied from map.
		auto result = find_if(entry.cbegin(), entry.cend(), [&](auto item) { return item->absolutePath == obj->absolutePath; });
		if (result == entry.cend()) return;
		entry.erase(result);
	}
	catch (int exception)
	{
		exception = 0;
		return;
	}
}

GameObject* GameObject::Find(const string& path)
{
	if (objects.empty()) return nullptr;

	// extract name part from path or from name
	string nameOnly = path;
	auto idx = path.find_last_of('/');
	if (idx != string::npos)
		nameOnly = path.substr(idx + 1);
	
	auto found = objects.find(nameOnly); 
	if (found == objects.end()) return nullptr;
	auto& entry = found->second;

	// if search by name only with no absolute path
	if (nameOnly == path) return entry.size() > 0 ? entry[0] : nullptr;

	// search by absolute path
	auto result = find_if(entry.cbegin(), entry.cend(), [&](auto item) { return item->absolutePath == path; });
	return result != entry.cend() ? *result : nullptr;
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

	GameObject::Add(gameObject);

	return gameObject;
}

GameObject::GameObject(const string& name, 
	const string& tag,
	GameObject* parent)
	: name(name), tag(tag), parent(parent),
	hideFlag(false), 
	transform(nullptr),
	absolutePath( parent == nullptr ? "/" + name : parent->absolutePath + "/" + name),
	screen(Screen::getInstance()),
	scene(Scene::getInstance()),

	inputManager(InputManager::getInstance())
{	
	transform = getOrAddComponent<Transform>();
}

GameObject::~GameObject() {}

