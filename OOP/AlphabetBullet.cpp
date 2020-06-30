#include "AlphabetBullet.h"


AlphabetBullet::AlphabetBullet(Screen& screen)
	: Bullet(screen)
{
}

void AlphabetBullet::OnDamage(GameObject* collider)
{
	setActive(true);
}