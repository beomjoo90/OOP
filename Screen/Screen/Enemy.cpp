#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Enemy.h"


Enemy::Enemy(int pos, const char* face) : GameObject(pos, face)
{}

Enemy::~Enemy()
{}

void Enemy::moveRandom()
{
	setPosition(getPosition() + rand() % 3 - 1);
}

void Enemy::update()
{
	moveRandom();
}


void Enemy::getHit()
{
	// we didn't implement anything.
}