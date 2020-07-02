#pragma once
#include "Enemy.h"
class AlphabetEnemy :
	public Enemy
{

public:
	AlphabetEnemy(Screen& screen);

	void OnDamage(GameObject* collider);
};
