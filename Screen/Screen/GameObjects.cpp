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

void GameObjects::add(GameObject* obj)
{
	for (int i = 0; i < capacity; i++)
	{
		if (!objects[i]) {
			objects[i] = obj;
			return;
		}
	}
}

void GameObjects::remove(GameObject* obj)
{
	for (int i = 0; i < capacity; i++) {
		if (objects[i] == obj) {
			objects[i] = nullptr;
			if (obj != nullptr)	{
				delete obj;
			}
			return;
		}
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

	for (int i = 0; i < capacity; i--)
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
