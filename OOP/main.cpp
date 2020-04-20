// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>


// commit practice
// 커밋 연습

void printSpaces(int i)
{
	for (int j = 0; j < i; j++)
		printf(" ");
}

void clear(char* canvas, int length)
{
	memset(canvas, ' ', length);
	canvas[length] = '\0';
}

void render(const char* canvas, int lastPosition)
{
	printf("%s\r", canvas);
}

struct Enemy {
	int		pos;
	char	shape[100];

	Enemy(const char* shape, int maxCount)
	{	
		strcpy(this->shape, shape);
		this->pos = rand() % (maxCount - strlen(this->shape));
	}

	bool isInside(int length)
	{
		return pos <= (length - strlen(shape)) && pos >= 0;
	}

	void moveRight()
	{
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	void draw(char* canvas, int maxCount)
	{
		if (isInside(maxCount) == false) return;
		strncpy(&canvas[pos], shape, strlen(shape));
	}
};

struct Bullet {
	int		pos;
	char	shape[100];
	bool	isFired;
	int		direction;

	Bullet(const char* shape)
	{
		this->pos = 0;
		strcpy(this->shape, shape);
		this->isFired = false;
		this->direction = 0;
	}

	bool isInside(int length)
	{
		return pos <= (length - strlen(shape)) && pos >= 0;
	}

	void moveRight()
	{
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	void update(int enemy_pos, const char* enemy_shape)
	{
		if (isFired == false) return;

		if (direction == 0)
			moveRight();
		else moveLeft();

		if ((direction == 0 && enemy_pos <= pos)
			|| (direction == 1 && pos < enemy_pos + strlen(enemy_shape)))
		{
			isFired = false;
		}
		
	}

	void draw(char* canvas, int maxCount)
	{
		if (isFired == false) return;
		if (isInside(maxCount) == false) return;
		strncpy(&canvas[pos], shape, strlen(shape));
	}
};

struct Player {
	int		pos;
	char	shape[100];

	// constructor 생성자
	Player(const char* shape, int maxCount)
	{
		strcpy(this->shape, shape);
		this->pos = rand() % (maxCount - strlen(this->shape));
	}

	bool isInside(int length)
	{
		return pos <= (length - strlen(shape)) && pos >= 0;
	}

	void fire(int enemy_pos, Bullet* bullet)
	{
		if (bullet == nullptr) return;
		if (bullet->isFired == true) return;

		bullet->isFired = true;
		bullet->pos = pos;
		if (pos < enemy_pos) {
			bullet->pos += strlen(shape) - 1;
			strcpy(bullet->shape, "-->");
			bullet->direction = 0;
		}
		else {
			strcpy(bullet->shape, "<--");
			bullet->direction = 1;
		}
	}

	void moveRight()
	{
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	void draw(char* canvas, int maxCount)
	{
		if (isInside(maxCount) == false) return;
		strncpy(&canvas[pos], shape, strlen(shape));
	}
};


int main()
{
	const int maxCount = 80;
	char canvas[maxCount + 1];
	Player	player{ "(o_o)", maxCount };
	Enemy   enemy{ "(*___*)", maxCount };
	Bullet	bullet{"->"};

	while (true)		
	{
		clear(canvas, maxCount);
				
		// update game objects (player, enemy ...)
		if (player.isInside(maxCount) == false || enemy.isInside(maxCount) == false)
			break; // check game loop termination condition

		if (_kbhit()) {
			int key = _getch();
			//printf("\n%c %d\n", key, key);
			switch (key) {
			case 'a':
				player.moveLeft();
				break;
			case 'd':
				player.moveRight();
				break;
			case ' ':
				player.fire(enemy.pos, &bullet);
				break;
			case 'w':
				enemy.moveRight();
				break;
			case 's':
				enemy.moveLeft();
				break;
			}
		}
		bullet.update(enemy.pos, enemy.shape);
		
		// draw game objects to a canvas (player, enemy ...)
		player.draw(canvas, maxCount);
		enemy.draw(canvas, maxCount);
		bullet.draw(canvas, maxCount);		
		
		// display canvas to a monitor
		render(canvas, maxCount);		
		Sleep(100);
	}
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
