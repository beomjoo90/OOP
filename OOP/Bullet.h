#pragma once
#include "GameObject.h"

class Screen;

class Bullet :
	public GameObject
{
	bool	isFired;
	int		direction;

public:
	Bullet(Screen& screen, const char* shape = "");

	~Bullet();

	bool checkFire();
	void setFire();
	void resetFire();

	void makeDirectionLeft();
	void makeDirectionRight();
	bool isDirectionRight();

	//overriding
	void update();

	//overriding
	void draw();
};

