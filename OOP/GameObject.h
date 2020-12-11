#pragma once
#include <string>
#include <vector>
#include <map>
#include "Utils.h"

using namespace std;

class Screen;
class Scene;
class Component;
class Transform;
class InputManager;

#include "Transform.h"

class GameObject
{
	string		name;
	string		absolutePath;
	string		tag;
	bool		hideFlag;

	GameObject(const string& name, const string& tag,
		GameObject* parent);

	static map<string, vector<GameObject*>> objects;
	
protected:
	Screen& screen;
	Scene& scene;
	InputManager& inputManager;

	GameObject* parent;
	Transform*	transform;
	vector<Component*> components;
	vector<GameObject*> children;

	friend class Component;
	friend class Scene;

	void internalStart();
	void internalUpdate();
	void internalDraw();

	static GameObject* Instantiate(const string& name,
		const string& tag = "Unknown", GameObject* parent = nullptr,
		const Position& pos = Position{ 0, 0 },
		const string& shape = "",
		const Position& size = Position{1, 1}
	);	
	
public:
	
	~GameObject();

	
	
	template<typename T>
	void addComponent()
	{
		auto t = new T(this);
		if (dynamic_cast<Component *>(t) == nullptr) {
			delete t;
			return;
		}
		components.push_back(t);
	}

	template<typename T>
	T* getComponent()
	{
		for (auto component : components) {
			auto found = dynamic_cast<T *>(component);
			if (found) return found;
		}
		for (auto child : children) {
			auto found = child->getComponent<T>();
			if (found) return found;
		}
		return nullptr;
	}

	template<typename T>
	T* getOrAddComponent()
	{
		auto t = getComponent<T>();
		if (t == nullptr) {
			addComponent<T>();
			t = getComponent<T>();
		}
		return t;
	}

	template<typename T>
	void removeComponent()
	{
		for (auto it = components.begin(); it != components.end(); )
		{
			if (dynamic_cast<T *>(*it)) it = components.erase(it);
			else it++;
		}
	}

	void add(GameObject* child) {
		if (child == nullptr) return;
		children.push_back(child);
	}

	const string getName() const { return name; }


	static GameObject* Find(const string& name);

	static void Add(GameObject* obj);

	static void Remove(GameObject* obj);

	
};

