#include "Bullet.h"

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

//overriding
void Bullet::draw()
{
	if (checkFire() == false) return;
	GameObject::draw();
}
