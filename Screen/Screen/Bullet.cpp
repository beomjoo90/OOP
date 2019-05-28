#define _CRT_SECURE_NO_WARNINGS
#include  <iostream>
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

Bullet::Bullet(int pos, const char* face, Screen& screen) :GameObject(pos, face, screen), isFiring(false)
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

void Bullet::process_input(int input, GameObject* objects[], int maxObjects)
{
	if (input != ' ') return;

	for (int i = 0; i < maxObjects; i++)
	{
		GameObject* obj = objects[i];
		if (!obj) continue;
		Player* player = dynamic_cast<Player*>(obj);
		if (!player) continue;
		fire(player->getPosition());
	}
}

void Bullet::update(GameObject* objects[], int maxObjects)
{
	if (!objects || maxObjects == 0) return;

	int enemy_pos = -1; // not found any enemy object
	for (int i = 0; i < maxObjects; i++)
	{
		GameObject* obj = objects[i];
		if (!obj) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(obj);
		if (!enemy) continue;
		enemy_pos = enemy->getPosition();
	}
	if (enemy_pos == -1) return; // not found any enemy

	if (isFiring == false) return;
	int pos = getPosition();
	if (pos < enemy_pos) {
		pos = pos + 1;
	}
	else if (pos > enemy_pos) {
		pos = pos - 1;
	}
	else {
		isFiring = false;
	}
	setPosition(pos);
}