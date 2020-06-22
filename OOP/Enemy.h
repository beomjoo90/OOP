#pragma once
#include "GameObject.h"

class Screen;

class Enemy :
	public GameObject
{

public:
	Enemy(Screen& screen, const char* shape);

	~Enemy() {}

	// overriding : 재정의
	void process_input(int key);

	// overriding
	void update();

	virtual void OnDamage() {}
};

class BlinkableEnemy :
	public Enemy
{
	bool isBlinking;
	int count;

public:
	BlinkableEnemy(Screen& screen, const char* shape)
		: Enemy(screen, shape), isBlinking(false), count(0)
	{}

	void setBlinking() { isBlinking = true; count = 10; }

	// overriding : 재정의
	void process_input(int key);

	// overriding
	void update();
	void OnDamage() { setBlinking(); }

	// overriding
	void draw();
};

