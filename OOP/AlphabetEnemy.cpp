#include <iostream>
#include "AlphabetEnemy.h"
#include "Screen.h"

AlphabetEnemy::AlphabetEnemy(Screen& screen)
	: Enemy(screen, screen.length(), "")
{
	static char buf[11];
	static const int alphabetRange = 2;

	int len = rand() % 10 + 1;

	int i = 0;
	for (; i < len; i++) 
		buf[i] = rand() % alphabetRange + (rand() % 2 ? 'A' : 'a');
	buf[i] = 0;
	setShape(buf);
	setPos(screen.length() - len);
}

void AlphabetEnemy::OnDamage(GameObject* collider)
{
	if (collider == nullptr) return;
	const char* opponent = collider->getShape();
	char shape[100];

	strcpy(shape, getShape());
	for (int i = 0; opponent[i] != '\0'; i++)
	{
		char c = opponent[i];
		for (int j = 0; j < strlen(shape);)
		{
			if (toupper(c) != toupper(shape[j])) {
				j++;
				continue;
			}
			int k = j + 1;
			while ((shape[k - 1] = shape[k]) != '\0') k++;
		}
	}
	setShape(shape);
	if (strlen(shape) == 0) setActive(false);
}
