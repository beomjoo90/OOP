#include "Bullet.h"
#include "Enemy.h"

//overriding
void Bullet::update()
{
	if (checkFire() == false) return;

	if (isInside() == false) {
		resetFire();
		return;
	}

	if (isDirectionRight())
		moveRight();
	else moveLeft();
}

void Bullet::OnDamage(GameObject* collider)
{
	resetFire();
}



//overriding
void Bullet::draw()
{
	if (checkFire() == false) return;
	GameObject::draw();
}
