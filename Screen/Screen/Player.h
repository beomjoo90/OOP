#pragma once
#include "GameObject.h"
class Player :
	public GameObject
{
public:
	Player(int pos, const char* face, Screen& screen);
	virtual ~Player();

	void moveLeft();

	void moveRight();

	void process_input(int input, GameObject* objects[], int maxObjects);
};

