#pragma once
#include "GameObject.h"

class Bullet :
	public GameObject
{
	bool isFiring;

	void fire(int player_pos)
	{
		isFiring = true;
		setPosition(player_pos);
	}

public:
	Bullet(int pos, const char* face, Screen& screen);
	virtual ~Bullet();

	void moveLeft();
	void moveRight();
	void draw();
	void process_input(int input, GameObject* objects[], int maxObjects);
	void update(GameObject* objects[], int maxObjects);
};

