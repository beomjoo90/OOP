#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "GameObject.h"



GameObject::GameObject(int pos, const char* face)
	: pos(pos), screen(Screen::getInstance()), objects(GameObjects::getInstance())
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

void GameObject::process_input(int input)
{	
}

void GameObject::draw()
{
	screen.draw(pos, face);
}

void GameObject::update()
{
}

void GameObject::addToList(GameObject* obj)
{
	objects.add(obj);
}

void GameObject::removeFromList(GameObject* obj)
{
	objects.remove(obj);
}

GameObjects& GameObject::getList()
{
	return objects;
}

