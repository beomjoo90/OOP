#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

Player::Player(int pos, const char* face) : GameObject(pos, face)
{
}

Player::~Player()
{
}

void Player::moveLeft()
{
	setPosition(getPosition() - 1);
}

void Player::moveRight()
{
	setPosition(getPosition() + 1);
}

void Player::process_input(int input)
{
	if (input == 'a') moveLeft();
	else if (input == 'd') moveRight();
	else if (input == ' ') {

		Enemy* enemy = dynamic_cast<Enemy *>(getList().getEnemy());
		if (enemy == nullptr) return;
		int pos = getPosition();
		int enemy_pos = enemy->getPosition();
		if (pos == enemy_pos) {
			enemy->getHit();
		}
		else {
			addToList(new Bullet{ getPosition(), "+", pos < enemy_pos ? Direction::Right : Direction::Left });
		}
	}
}