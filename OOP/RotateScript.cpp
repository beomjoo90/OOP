#include <iostream>
#include "Utils.h"
#include "GameObject.h"
#include "RotateScript.h"
#include "InputManager.h"

RotateScript::RotateScript(GameObject* gameObject)
	: Component(gameObject), shapeData(nullptr), transform(nullptr)
{}

RotateScript::~RotateScript() {
	if (shapeData == nullptr) return;
	delete[] shapeData;
}

void RotateScript::start() {
	transform = getTransform();
	string shape = transform->getShape();	
	Position size = transform->getSize();
	const char* p = shape.c_str();
	if (shapeData == nullptr) shapeData = new char[size.x*size.y + 1];
	strncpy(shapeData, p, size.x*size.y);
	
	GameObject* block = GameObject::Find("block");
}

void RotateScript::rotate() {
	string shape = transform->getShape();
	Position size = transform->getSize();
	const char* p = shape.c_str();
	int w = size.x; int h = size.y;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++)
			shapeData[(w - 1 - x)*h + y] = p[y*w + x];
	transform->setShape(shapeData);
	transform->setSize(Position{ h, w });
}

void RotateScript::update() {
	if (getInputManager().GetKeyDown(VK_UP)) rotate();
	if (getInputManager().GetKeyDown(VK_SPACE)) {
		GameObject::Destroy(gameObject);
	}
}
