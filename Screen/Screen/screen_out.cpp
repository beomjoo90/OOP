#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <Windows.h>
#include <conio.h>

using namespace std;

#define SCREEN_SIZE	79

struct GameObject {
	int		pos;
	char	shape[20];

	void	draw(char *screen) const
	{
		strncpy(screen + pos, shape, strlen(shape));
	}
};

struct Bullet {
	struct GameObject	myData;

	void	draw(char *screen, const struct GameObject *player, const struct GameObject *enemy) const
	{
		if (myData.pos == -1) return;

		if (screen == NULL || player == NULL || enemy == NULL) return;
		
		if (player->pos < enemy->pos) {
			strncpy(screen + myData.pos, ">", strlen(">"));
		}
		else if (player->pos > enemy->pos) {
			strncpy(screen + myData.pos, "<", strlen("<"));
		}
	}
};


void clear_screen(char *screen)
{
	memset(screen, ' ', SCREEN_SIZE);
}

void draw(char *screen, const struct GameObject* player, const struct GameObject* enemy, const struct Bullet bullets[], int bullets_len)
{
	if (screen == NULL) return;

	player->draw(screen);
	enemy->draw(screen);

	for (int i = 0; i < bullets_len; i++)
	{
		bullets[i].draw(screen, player, enemy);
	}
}

void update(const struct GameObject* player, const struct GameObject* enemy, struct Bullet bullets[], int bullets_len)
{
	for (int i = 0; i < bullets_len; i++)
	{
		if (bullets[i].myData.pos == -1) continue;

		if (bullets[i].myData.pos == enemy->pos)
		{
			bullets[i].myData.pos = -1;
			continue;
		}

		if (bullets[i].myData.pos < 0 || bullets[i].myData.pos >= SCREEN_SIZE)
		{
			bullets[i].myData.pos = -1;
			continue;
		}

		if (player->pos < enemy->pos)		bullets[i].myData.pos++;
		else if (player->pos > enemy->pos)	bullets[i].myData.pos--;
	}
}

void render(char *screen)
{
	screen[SCREEN_SIZE] = '\0';
	cout << screen << '\r';
	Sleep(33);
}

void process_input(struct GameObject* player, struct GameObject* enemy, struct Bullet bullets[], int bullets_len)
{
	if (_kbhit() == 0) return;

	int ch = _getch();

	if (ch == 224)
	{
		ch = _getch();
		if (ch == 75) player->pos--;
		if (ch == 77) player->pos++;
		if (ch == 80) enemy->pos++;
		if (ch == 72) enemy->pos--;
	}
	else if (ch == ' ')
	{
		for (int i = 0; i < bullets_len; i++)
		{
			if (bullets[i].myData.pos != -1) continue;

			// found a bullet ready to shoot
			bullets[i].myData.pos = player->pos;
			break; // exit from the loop
		}
	}
}

int main()
{	
	char screen[SCREEN_SIZE + 1];
	const int bullets_len = 30;
	struct GameObject player;
	struct GameObject enemy;
	struct Bullet bullets[bullets_len];

	strcpy(player.shape, "^_^");
	player.pos = rand() % SCREEN_SIZE;
	strcpy(enemy.shape, "*__*");
	enemy.pos = rand() % SCREEN_SIZE;
	for (int i = 0; i < bullets_len; i++)
	{
		strcpy(bullets[i].myData.shape, ">");
		bullets[i].myData.pos = -1;
	}
	
	while (1)
	{
		clear_screen(screen);
		process_input(&player, &enemy, bullets, bullets_len);
		draw(screen, &player, &enemy, bullets, bullets_len);
		update( &player, &enemy, bullets, bullets_len);
		render(screen);			
	}
	return 0;

}