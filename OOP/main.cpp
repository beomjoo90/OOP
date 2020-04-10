// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
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


void draw(char* canvas, int pos, const char* source)
{
	strncpy(&canvas[pos], source, strlen(source));
}

void render(const char* canvas, int lastPosition)
{
	printf("%s\r", canvas);
}

bool isInside(char* shape, int pos, int length)
{
	return pos <= (length - strlen(shape)) && pos >= 0;
}

int main()
{
	const int maxCount = 80;
	char canvas[maxCount + 1];
	int player_pos = 0;
	char player_shape[maxCount] = "(o_o)";
	int enemy_pos = 0;
	char enemy_shape[maxCount] = "(*______*)";

	//srand(time(nullptr));
	player_pos = rand() % (maxCount - strlen(player_shape));
	enemy_pos = rand() % (maxCount - strlen(enemy_shape));
	
	while ( isInside(player_shape, player_pos, maxCount) 
		&& isInside(enemy_shape, enemy_pos, maxCount) )		
	{
		clear(canvas, maxCount );

		draw(canvas, player_pos, player_shape);
		if (rand() % 2) player_pos++;
		else player_pos--;
		
		draw(canvas, enemy_pos, enemy_shape);
		if (rand() % 2) enemy_pos++;
		else enemy_pos--;
		

		render(canvas, maxCount);		
		Sleep(100);
	}
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
