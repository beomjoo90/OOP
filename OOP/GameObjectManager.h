#pragma once

class GameObject;

class GameObjectManager
{
	GameObject** gos;
	int          capacity;

public:
	GameObjectManager(int capacity = 20);
	~GameObjectManager();

	void add(GameObject* obj);
	void remove(GameObject* obj);

	GameObject** getGameObjects() { return gos; }
	int getCapacity() { return capacity; }
};

