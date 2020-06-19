#include "Bullet.h"

Bullet::Bullet(Screen& screen, const char* shape)
	: GameObject(screen, -1, shape), isFired(false), direction(0)
{}

Bullet::~Bullet() {}

bool Bullet::checkFire()
{
	return isFired == true;
}
void Bullet::setFire()
{
	isFired = true;
}
void Bullet::resetFire()
{
	isFired = false;
}

void Bullet::makeDirectionLeft() { direction = 1; }
void Bullet::makeDirectionRight() { direction = 0; }
bool Bullet::isDirectionRight() { return direction == 0; }

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
