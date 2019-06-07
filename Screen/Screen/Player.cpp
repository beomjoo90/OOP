#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Player.h"

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

void Player::process_input(int input, GameObject* objects[], int maxObjects)
{
	if (input == 'a') moveLeft();
	else if (input == 'd') moveRight();
}