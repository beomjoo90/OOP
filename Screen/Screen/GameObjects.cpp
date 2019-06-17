#include <stdio.h>
#include <conio.h>
#include "Enemy.h"
#include "GameObjects.h"

GameObjects* GameObjects::instance = nullptr;

GameObjects& GameObjects::getInstance()
{
	if (instance == nullptr) {
		instance = new GameObjects(100);
	}
	return *instance;
}

GameObjects::GameObjects(int capacity=100)
	:capacity(capacity), objects( new GameObject*[capacity])
{
	for (int i = 0; i < capacity; i++)
	{
		objects[i] = nullptr;
	}
}

GameObjects::~GameObjects()
{
	if (objects == nullptr) return;
	for (int i = 0; i < capacity; i++)
	{
		GameObject* obj = objects[i];
		if (obj == nullptr) continue;
		delete obj;
		objects[i] = nullptr;
	}
	delete[] objects;
	objects = nullptr;
	capacity = 0;
}

int GameObjects::find(GameObject* obj)
{
	for (int i = 0; i < capacity; i++) {
		if (objects[i] == obj) return i;
	}
	return -1;
}

void GameObjects::add(GameObject*	obj)
{
	int index = find(obj);
	if (index >= 0) return;
	index = find(nullptr);
	if (index == -1) return;
	objects[index] = obj;
}

void GameObjects::remove(GameObject* obj)
{
	int index = find(obj);
	if (index == -1) return;
	objects[index] = nullptr;
	if (obj) {
		delete obj;
	}
}

void GameObjects::update()
{
	if (_kbhit())
	{
		int c = _getch();
		for (int i = 0; i < capacity; i++)
		{
			GameObject* obj = objects[i];
			if (!obj) continue;
			obj->process_input(c);
		}
	}
	for (int i = 0; i < capacity; i++)
	{
		GameObject* obj = objects[i];
		if (!obj) continue;
		obj->draw();
	}

	for (int i = 0; i < capacity; i++)
	{
		GameObject* obj = objects[i];
		if (!obj) continue;
		obj->update();
	}

}


GameObject* GameObjects::getEnemy()
{
	for (int i = 0; i < capacity; i++)
	{
		GameObject* obj = objects[i];
		if (!obj) continue;
		if (dynamic_cast<Enemy *>(obj)) return obj;
	}
	return nullptr;
}
