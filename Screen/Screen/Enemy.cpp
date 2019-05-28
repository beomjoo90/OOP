#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Enemy.h"


Enemy::Enemy(int pos, const char* face, Screen& screen) : GameObject(pos, face, screen)
{}

Enemy::~Enemy()
{}

void Enemy::moveRandom()
{
	setPosition(getPosition() + rand() % 3 - 1);
}

void Enemy::update(GameObject* objects[], int maxObjects)
{
	if (!objects || maxObjects == 0) return;
	moveRandom();
}