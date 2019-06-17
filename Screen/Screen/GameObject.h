#pragma once
#include "Screen.h"
#include "GameObjects.h"

enum Direction {
	Left = 0,
	Right
};

class GameObject
{
	int pos;
	char face[20];
	Screen& screen;
	GameObjects& objects;
public:
	GameObject(int pos, const char* face);
	
	virtual ~GameObject();

	int getPosition();

	void setPosition(int pos);

	virtual void process_input(int input);

	virtual void draw();

	virtual void update();

	void addToList(GameObject* obj);
	void removeFromList(GameObject* obj);

	GameObjects& getList();

};

