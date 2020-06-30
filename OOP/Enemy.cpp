#include <iostream>

#include "Enemy.h"
#include "Screen.h"
#include "Bullet.h"
#include "Player.h"

Enemy::Enemy(Screen& screen, int pos, const char* shape)
	: GameObject(screen, pos, shape)
{}

// overriding : 재정의
void Enemy::process_input(int key)
{
	switch (key) {
	case 'w': moveRight(); break;
	case 's': moveLeft(); break;
	}
}

void Enemy::OnDamage(GameObject* collider)
{
	setActive(false);
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
		if (bullet->isColliding(this) == true) {
			OnDamage(bullet);
			bullet->OnDamage(this);
		}
	}
	
	maxGameObjects = getMaxGameObjects();
	for (int i = 0; i < maxGameObjects; ++i)
	{
		GameObject* obj = gos[i];
		if (obj == nullptr) continue;
		Player* player = dynamic_cast<Player*>(obj);
		if (player == nullptr) continue;
		int player_pos = player->getPos();
		if (pos < player_pos) {
			moveRight();
		}
		else if (pos > player_pos) {
			moveLeft();
		}
		else {
			setActive(false);
		}

	}
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

// overriding
void BlinkableEnemy::draw()
{
	if (isBlinking == false) {
		GameObject::draw();
		return;
	}
	if (count % 2 == 0) GameObject::draw();
}
