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
	auto mainPanel = GameObject::Instantiate("mainPanel");
	mainPanel->addComponent<PanelScript>();
	
	auto nextPanel = GameObject::Instantiate("nextPanel");

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
