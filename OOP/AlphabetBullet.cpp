#include <iostream>
#include "AlphabetBullet.h"


AlphabetBullet::AlphabetBullet(Screen& screen)
	: Bullet(screen)
{
	static char buf[2];
	const int alphabetRange = 2;
	buf[0] = rand() % alphabetRange + (rand() % 2 ? 'A' : 'a');
	buf[1] = 0;
	setShape(buf);
}

void AlphabetBullet::OnDamage(GameObject* collider)
{
	setActive(true);
}