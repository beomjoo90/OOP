#include <iostream>
#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(int capacity)
	: capacity{ capacity }, gos{ (GameObject**)malloc(sizeof(GameObject*)*capacity) }
{
	for (int i = 0; i < capacity; i++)
		gos[i] = (GameObject*)nullptr;
}
GameObjectManager::~GameObjectManager() {

	for (int i = 0; i < capacity; i++)
	{
		if (gos[i] == nullptr) continue;
		delete gos[i];
		gos[i] = (GameObject*)nullptr;
	}
	free(gos);
	gos = (GameObject**)nullptr;
	capacity = 0;
}

void GameObjectManager::add(GameObject* obj)
{
	if (obj == nullptr) return;
	// obj != nullptr
	for (int i = 0; i < capacity; i++)
	{
		if (gos[i] != nullptr) continue;
		// gos[i] == nullptr
		gos[i] = obj;
		return;
	}
	// i == capacity
	gos = (GameObject**)realloc(gos, sizeof(GameObject*) * 2 * capacity);
	for (int i = capacity; i < 2 * capacity; i++)
	{
		gos[i] = nullptr;
	}
	gos[capacity] = obj;
	capacity *= 2;
}

void GameObjectManager::remove(GameObject* obj)
{
	if (obj == nullptr) return;
	for (int i = 0; i < capacity; i++)
	{
		if (gos[i] == obj) {
			gos[i] = nullptr;
			return;
		}
	}
}

GameObject** GameObjectManager::getGameObjects() { return gos; }
int GameObjectManager::getCapacity() { return capacity; }
