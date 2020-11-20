#include "Scene.h"
#include "GameObject.h"
#include "PanelScript.h"

Scene::Scene() {
}

Scene& Scene::getInstance() {
	static Scene instance;
	return instance;
}

void Scene::start() {
	auto mainPanel = GameObject::Instantiate("mainPanel", "panel", nullptr,
		Position{ 1, 1 }, "", Position{ 70, 30 });
	mainPanel->addComponent<PanelScript>();
	auto movingBlock = GameObject::Instantiate("tetris block", "block", mainPanel,
		Position{ 4, 0 }, "\xdb  \xdb\xdb\xdb  \xdb", Position{ 3, 3 } );

	auto nextPanel = GameObject::Instantiate("nextPanel");
	auto staticBlock = GameObject::Instantiate("next block", "block", nextPanel);

	auto scorePanel = GameObject::Instantiate("scorePanel");

	for (auto gameObject : gameObjects) gameObject->internalStart();
}

void Scene::add(GameObject* go) {
	if (go == nullptr) return;
	gameObjects.push_back(go);
}

void Scene::remove(GameObject* go) {
	if (go == nullptr) return;
	for (auto it = gameObjects.begin(); it != gameObjects.end();)
	{
		auto obj = *it;
		if (obj == go) {
			it = gameObjects.erase(it);
			delete obj;
		}
		else			it++;
	}
}

void Scene::update() {
	for (auto gameObject : gameObjects) gameObject->internalUpdate();
}

void Scene::draw() {
	for (auto gameObject : gameObjects) gameObject->internalDraw();
}
