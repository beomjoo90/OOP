#include <iostream>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Screen.h"


Bullet* Player::findUnusedBullet()
{
	GameObject** gos = getGameObjects();
	int maxGameObjects = getMaxGameObjects();
	for (int i = 0; i < maxGameObjects; i++)
	{
		GameObject* obj = gos[i];
		if (obj == nullptr) continue;

		// obj != nullptr
		Bullet* bullet = dynamic_cast<Bullet*>(obj);
		if (bullet == nullptr) continue;

		// bullet != nullptr
		if (bullet->checkFire() == true) continue;

		// bullet != nullptr && bullet->checkFire() == false
		return bullet;
	}
	return nullptr;
}

void Player::fire()
{
	Bullet* bullet = findUnusedBullet();
	if (bullet == nullptr) {
		bullet = new Bullet{ getScreen() };
	}
	// bullet != nullptr
	int pos = getPos();

	// bullet != nullptr && bullet->checkFire() == false
	Enemy* enemy = nullptr;
	Enemy* closest = nullptr;
	int closestDistance = getScreen().length();

	// find the closest enemy from game objects array.
	GameObject** gos = getGameObjects();
	int maxGameObjects = getMaxGameObjects();
	for (int i = 0; i < maxGameObjects; i++)
	{
		GameObject* obj = gos[i];
		if (obj == nullptr) continue;
		enemy = dynamic_cast<Enemy*>(obj);
		if (enemy == nullptr) continue;
		int dist = abs(enemy->getPos() - pos);
		if (closest == nullptr || (dist < closestDistance)) {
			closest = enemy;
			closestDistance = dist;
		}
	}

	int enemy_pos = -1;
	if (closest == nullptr) {
		bool isLeft = rand() % 2 ? false : true;
		enemy_pos = getScreen().length() - 1;
		if (isLeft == true) enemy_pos = 0;
	}
	else {
		enemy_pos = closest->getPos();
	}
	bullet->setFire();

	bullet->setPos(pos);
	bullet->setShape("<--");
	bullet->makeDirectionLeft();
	if (pos < enemy_pos) {
		bullet->setPos(bullet->getPos() + (int)strlen(getShape()) - 1);
		bullet->setShape("-->");
		bullet->makeDirectionRight();
	}
}

// constructor »ý¼ºÀÚ
Player::Player(Screen& screen, const char* shape)
	: GameObject(screen, rand() % (screen.length() - strlen(shape)), shape)
{}

Player::~Player() {}

// overriding
void Player::process_input(int key)
{
	Bullet* bullet = nullptr;

	switch (key) {
	case 'a': moveLeft(); break;
	case 'd': moveRight(); break;
	case ' ':
		fire();
		break;
	}
}
