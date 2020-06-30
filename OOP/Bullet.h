#pragma once
#include "GameObject.h"

class Screen;

class Bullet :
	public GameObject
{
	bool	isFired;
	int		direction;

public:
	Bullet(Screen& screen, int pos = -1, const char* shape = "")
		: GameObject(screen, pos, shape), isFired(false), direction(0)
	{}

	~Bullet() {}

	bool checkFire() { 	return isFired; }
	void setFire() { isFired = true; }
	void resetFire() { isFired = false; }

	void makeDirectionLeft() { direction = 1;  }
	void makeDirectionRight() { direction = 0; }
	bool isDirectionRight() { return direction == 0; }

	//overriding
	void update();

	//overriding
	void draw();

	void OnDamage(GameObject* collider);
};
