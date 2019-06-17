#pragma once
#include "GameObject.h"

class Bullet :
	public GameObject
{
	bool isFiring;
	Direction direction;

public:
	Bullet(int pos, const char* face, Direction direction);
	virtual ~Bullet();

	void moveLeft();
	void moveRight();
	void draw();
	void process_input(int input);
	void update();
};

