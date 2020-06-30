#pragma once
#include "Enemy.h"
class AlphabetEnemy :
	public Enemy
{

public:
	AlphabetEnemy(Screen& screen);

	void update();
	void OnDamage(GameObject* collider);
};
