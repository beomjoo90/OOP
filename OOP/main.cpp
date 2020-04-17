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

bool isInside(const char* shape, int pos, int length)
{
	return pos <= (length - strlen(shape)) && pos >= 0;
}

void draw(char* canvas, int pos, const char* source, int length)
{
	if (isInside(source, pos, length) == false) return;
	strncpy(&canvas[pos], source, strlen(source));
}

void render(const char* canvas, int lastPosition)
{
	printf("%s\r", canvas);
}

struct Player {
	int		pos;
	char	shape[100];

	// constructor 생성자
	Player(const char* shape, int maxCount)
	{
		strcpy(this->shape, shape);
		this->pos = rand() % (maxCount - strlen(this->shape));
	}
};

struct Enemy {
	int		pos;
	char	shape[100];

	Enemy(const char* shape, int maxCount)
	{	
		strcpy(this->shape, shape);
		this->pos = rand() % (maxCount - strlen(this->shape));
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
};

int main()
{
	const int maxCount = 80;
	char canvas[maxCount + 1];
	Player	player{ "(o_o)", maxCount };
	Enemy   enemy{ "(*___*)", maxCount };
	Bullet	bullet{"->"};


	/*
	while (true)		
	{
		clear(canvas, maxCount);
				
		// update game objects (player, enemy ...)
		if (isInside(player_shape, player_pos, maxCount) == false
			|| isInside(enemy_shape, enemy_pos, maxCount) == false)
			break; // check game loop termination condition

		if (_kbhit()) {
			int key = _getch();
			//printf("\n%c %d\n", key, key);
			switch (key) {
			case 'a':
				player_pos--;
				break;
			case 'd':
				player_pos++;
				break;
			case ' ':
				if (isFired == true) 
					break;
				isFired = true;
				bullet_pos = player_pos;
				if (player_pos < enemy_pos) {
					bullet_pos += strlen(player_shape)-1;
					strncpy(bullet_shape, "-->", strlen("-->"));
					direction = 0;
				}
				else {
					strncpy(bullet_shape, "<--", strlen("<--"));
					direction = 1;
				}
				break;
			case 'w':
				enemy_pos++;
				break;
			case 's':
				enemy_pos--;
				break;
			}
		}
		if (isFired == true) {
			if (direction == 0)
				bullet_pos++;
			else bullet_pos--;

			if ( (direction == 0 && enemy_pos <= bullet_pos)
				|| (direction == 1 && bullet_pos < enemy_pos+strlen(enemy_shape) ) )
			{
				isFired = false;
			}
		}


		
		// draw game objects to a canvas (player, enemy ...)
		draw(canvas, player_pos, player_shape, maxCount);
		draw(canvas, enemy_pos, enemy_shape, maxCount);
		if (isFired == true)
			draw(canvas, bullet_pos, bullet_shape, maxCount);
		
		// display canvas to a monitor
		render(canvas, maxCount);		
		Sleep(100);
	}
	*/
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
