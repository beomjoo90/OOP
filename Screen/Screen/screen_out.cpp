#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <Windows.h>
#include <conio.h>
#include <time.h>  // or simply <ctime>

#define SCREEN_SIZE		79

using namespace std;

class GameObject {
	char 	shape[16];
	int		pos;

public:
	GameObject(const char *shape, int pos) : pos(pos) { strcpy(this->shape, shape); }

	void SetShape(const char *shape) { strcpy(this->shape, shape); }

	void SetPos(int pos) { this->pos = pos; }
	int  GetPos() const { return this->pos; }

	void IncreasePos() { this->pos++; }
	void DecreasePos() { this->pos--; }

	void Update() {}

	void Draw(char *canvas) const
	{
		if (pos < 0 || pos >= SCREEN_SIZE) return;

		strncpy(canvas + pos, shape, strlen(shape));
	}
};

class Player : public GameObject {

public:
	Player() : GameObject("^_^", rand() % SCREEN_SIZE ) {}
};

class Enemy : public GameObject {
	int hp;
	int isAlive;

public:
	Enemy(int hp) : hp(hp), isAlive(true), GameObject("*_*", rand() % SCREEN_SIZE ) {}

	void OnAttacked()
	{
		if (hp > 0) --hp;
		if (hp <= 0) isAlive = false;
	}

	// overriding
	void Draw(char *canvas) const
	{
		if (isAlive == false) return;		
		GameObject::Draw(canvas);
	}
};

class Bullet : public GameObject {
	bool 		isFired;
	Enemy* 		target;
	clock_t		firedTicks;

	void reset() { isFired = false; firedTicks = 0l; target = nullptr; }

public:
	Bullet() : isFired(false), GameObject(">", 0), target(nullptr), firedTicks(0l) {}

	void Fire(const Player& player, Enemy& enemy)
	{
		int player_pos = player.GetPos();
		int target_pos = enemy.GetPos();

		if (player_pos == target_pos) return;

		if (player_pos < target_pos) {
			SetShape(">");
		}
		else {
			SetShape("<");
		}

		SetPos(player_pos);
		target = &enemy;
		firedTicks = clock();
	}

	bool IsUsed() { return isFired || firedTicks != 0l; }

	//overriding
	void Update()
	{
		if (!target) return;

		if (isFired == false) {
			if (firedTicks == 0l) return;

			// 만일 다음 라인을 comment out한다면 5초후 발사 기능이 사라짐.
			if ( ((clock() - firedTicks) / CLOCKS_PER_SEC) < 5) return;

			isFired = true;
		}

		int pos = GetPos();
		int target_pos = target->GetPos();

		if (pos < 0 || pos >= SCREEN_SIZE)
		{
			reset();
			return;
		}

		if (pos == target_pos) 
		{
			target->OnAttacked();
			reset();
			return;
		}

		if (pos < target_pos) {
			IncreasePos();
		} else {
			DecreasePos();
		}
	}

	// overriding
	void Draw(char *canvas) const
	{
		if (isFired == false || target == nullptr) return;

		GameObject::Draw(canvas);
	}
};

void clear_screen(char *canvas)
{
	memset(canvas, ' ', SCREEN_SIZE);
	canvas[SCREEN_SIZE] = '\0';
}

void process_input(Player& player, Enemy& enemy, Bullet bullets[], int bullets_len)
{
	if (_kbhit() == 0) return;

	int ch = _getch();
	if (ch == 224)
	{
		ch = _getch();
		switch (ch)
		{
		case 75:
			player.IncreasePos();
			break;
		case 77:
			player.DecreasePos();
			break;
		case 72:
			enemy.IncreasePos();
			break;
		case 80:
			enemy.DecreasePos();
			break;
		}
	}
	else if (ch == ' ')
	{
		for (int i = 0; i < bullets_len; i++)
		{
			if (bullets[i].IsUsed() == false)
			{
				bullets[i].Fire(player, enemy);
				break; // exit from for statement
			}
		}
	}
}

void draw(char *canvas, const Player& player, const Enemy& enemy, const Bullet bullets[], int bullets_len)
{
	if (canvas == NULL) return;

	player.Draw(canvas);
	enemy.Draw(canvas);
	for (int i= 0; i < bullets_len; i++)
	{
		bullets[i].Draw(canvas);
	}
}

void update(Player& player, Enemy& enemy, Bullet bullets[], int bullets_len)
{
	player.Update();
	enemy.Update();
	for (int i = 0; i < bullets_len; i++)
	{
		bullets[i].Update();
	}
}

void render(char *canvas)
{
	canvas[SCREEN_SIZE] = '\0';
	cout << canvas << '\r';
	Sleep(33);
}

int main()
{
	char	canvas[SCREEN_SIZE + 1];
	Player player;	
	Enemy  enemy(10);	
	const int bullets_len = 5;
	Bullet bullets[bullets_len];

	while (1)
	{
		clear_screen(canvas);
		process_input(player, enemy, bullets, bullets_len);
		draw(canvas, player, enemy, bullets, bullets_len);
		update(player, enemy, bullets, bullets_len);
		render(canvas);
	}
	return 0;
}