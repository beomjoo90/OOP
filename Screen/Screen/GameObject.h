#pragma once
#include "Screen.h"

class GameObject
{
	int pos;
	char face[20];
	Screen& screen;
public:
	GameObject(int pos, const char* face);
	
	virtual ~GameObject();

	int getPosition();

	void setPosition(int pos);

	virtual void process_input(int input, GameObject* objects[], int maxObjects);

	virtual void draw();

	virtual void update(GameObject* objects[], int maxObjects);

};

