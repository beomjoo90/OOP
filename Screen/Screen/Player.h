#pragma once
#include "GameObject.h"
class Player :
	public GameObject
{
public:
	Player(int pos, const char* face);
	virtual ~Player();

	void moveLeft();

	void moveRight();

	void process_input(int input);
};

