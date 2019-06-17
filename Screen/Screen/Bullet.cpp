#define _CRT_SECURE_NO_WARNINGS
#include  <iostream>
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

Bullet::Bullet(int pos, const char* face, Direction direction=Direction::Left) : GameObject(pos, face), isFiring(true), direction(direction)
{
}

Bullet::~Bullet() {}

void Bullet::moveLeft()
{
	setPosition(getPosition() - 1);
}

void Bullet::moveRight()
{
	setPosition(getPosition() + 1);
}

void Bullet::draw()
{
	if (isFiring == false) return;
	GameObject::draw();
}

void Bullet::process_input(int input)
{
	if (input != ' ') return;
}

void Bullet::update()
{
	if (isFiring == false) {
		getList().remove(this);
		return;
	}
	GameObject* enemy = getList().getEnemy();
	if (!enemy) return;

	int pos = getPosition();
	if (pos == enemy->getPosition()) {
		dynamic_cast<Enemy *>(enemy)->getHit();
		isFiring = false;
		return;
	}
	if (direction == Direction::Left) moveLeft();
	else moveRight();
}