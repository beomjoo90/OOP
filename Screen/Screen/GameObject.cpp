#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "GameObject.h"


GameObject::GameObject(int pos, const char* face)
	: pos(pos), screen(Screen::getInstance())
{
	strcpy(this->face, face);
}

GameObject::~GameObject() {}

int GameObject::getPosition()
{
	return pos;
}

void GameObject::setPosition(int pos)
{
	this->pos = pos;
}

void GameObject::process_input(int input, GameObject* objects[], int maxObjects)
{
	if (!objects || maxObjects == 0) return;
}

void GameObject::draw()
{
	screen.draw(pos, face);
}

void GameObject::update(GameObject* objects[], int maxObjects)
{
	if (!objects || maxObjects == 0) return;
}

