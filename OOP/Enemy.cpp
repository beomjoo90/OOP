#include <iostream>

#include "Enemy.h"
#include "Screen.h"
#include "Bullet.h"

Enemy::Enemy(Screen& screen, const char* shape)
	: GameObject(screen, rand() % (screen.length() - (int)strlen(shape)), shape)
{}

Enemy::~Enemy() {}

// overriding : 재정의
void Enemy::process_input(int key)
{
	switch (key) {
	case 'w': moveRight(); break;
	case 's': moveLeft(); break;
	}
}

// overriding
void Enemy::update()
{
	if (isInside() == false) return;

	GameObject** gos = getGameObjects();
	int maxGameObjects = getMaxGameObjects();
	int pos = getPos();
	for (int i = 0; i < maxGameObjects; ++i)
	{
		GameObject* obj = gos[i];
		if (obj == nullptr) continue;
		Bullet* bullet = dynamic_cast<Bullet*>(obj);
		if (bullet == nullptr) continue;
		if (bullet->checkFire() == false) continue;
		if ((bullet->isDirectionRight() == true && bullet->getPos() + strlen(bullet->getShape()) == pos)
			|| (bullet->isDirectionRight() == false && pos + strlen(getShape()) == bullet->getPos())) {
			OnDamage();
			bullet->resetFire();
		}
	}
}

void Enemy::OnDamage() {}



BlinkableEnemy::BlinkableEnemy(Screen& screen, const char* shape)
	: Enemy(screen, shape), isBlinking(false), count(0)
{}

void BlinkableEnemy::setBlinking() {
	isBlinking = true;
	count = 10;
}

// overriding : 재정의
void BlinkableEnemy::process_input(int key)
{
	switch (key) {
	case 'j': moveLeft(); break;
	case 'k': moveRight(); break;
	}
}

// overriding
void BlinkableEnemy::update()
{
	Enemy::update();
	if (isBlinking == true) {
		count--;
		if (count == 0) {
			isBlinking = false;
		}
	}
}

void BlinkableEnemy::OnDamage()
{
	setBlinking();
}

// overriding
void BlinkableEnemy::draw()
{
	if (isBlinking == false) {
		GameObject::draw();
		return;
	}
	if (count % 2 == 0) GameObject::draw();
}
