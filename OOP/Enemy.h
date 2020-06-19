#pragma once
#include "GameObject.h"

class Screen;

class Enemy :
	public GameObject
{

public:
	Enemy(Screen& screen, const char* shape);
	~Enemy();

	// overriding : 재정의
	void process_input(int key);

	// overriding
	void update();

	virtual void OnDamage();
};

class BlinkableEnemy :
	public Enemy
{
	bool isBlinking;
	int count;

public:
	BlinkableEnemy(Screen& screen, const char* shape);

	void setBlinking();

	// overriding : 재정의
	void process_input(int key);

	// overriding
	void update();
	void OnDamage();

	// overriding
	void draw();
};

