#pragma once
#include <string>
#include <vector>
#include "Utils.h"

using namespace std;

class Screen;
class Scene;
class Component;
class Transform;
class InputManager;

class GameObject
{
	string		name;
	string		tag;
	bool		hideFlag;

	GameObject(const string& name, const string& tag,
		GameObject* parent);

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
		const Position& pos = Position{ 0, 0 }
	);	
	
public:
	
	~GameObject();

	
	template<typename T>
	void addComponent()
	{
		auto t = new T(this);
		if (dynamic_cast<T *>(t) == nullptr) {
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
		return nullptr;
	}

	const string getName() const { return name; }
};

