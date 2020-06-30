#pragma once
#include "Bullet.h"
class AlphabetBullet :
	public Bullet
{
public:
	AlphabetBullet(Screen& screen);

	void OnDamage(GameObject* collider);
};
