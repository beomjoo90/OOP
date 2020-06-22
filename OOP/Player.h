#pragma once
#include "GameObject.h"

class Bullet;

class Player :
	public GameObject
{
	Bullet* findUnusedBullet();

	void fire();

public:
	// constructor »ý¼ºÀÚ
	Player(Screen& screen, const char* shape);

	// overriding
	void process_input(int key);
};

