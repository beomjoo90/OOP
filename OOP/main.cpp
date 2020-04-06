// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>


// commit practice
// 커밋 연습

void printSpaces(int i)
{
	for (int j = 0; j < i; j++)
		printf(" ");
}

void draw(char* canvas, int pos, const char* source)
{
	strncpy(&canvas[pos], source, strlen(source));
}

int main()
{
	const int maxCount = 80;
	char canvas[maxCount + 1];
	int player_pos = 0;
	char player_shape[100] = "(o_o)";
	int enemy_pos = maxCount - strlen("(*______*)");
	char enemy_shape[100] = "(*______*)";
	
	while (player_pos < (maxCount - strlen(player_shape)) && enemy_pos >= 0)
	{
		memset(canvas, ' ', maxCount);		

		// player 그림을 canvas 공간에 player 위치에 복사
		draw(canvas, player_pos, player_shape);
		player_pos++;

		draw(canvas, enemy_pos, enemy_shape);
		enemy_pos--;

		canvas[maxCount] = '\0';
		printf("%s\r", canvas);
		Sleep(100);
	}
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
