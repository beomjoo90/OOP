// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>

void printSpaces(int i)
{
	for (int j = 0; j < i; j++)
		printf(" ");
}

int main()
{
	const int maxCount = 80;
	char canvas[maxCount + 1];
	int player_pos = 0;
	char player_shape[6] = "(o_o)";
	int enemy_pos = 75;
	char enemy_shape[6] = "(*_*)";

	while (player_pos < maxCount && enemy_pos >= 0)
	{
		for (int i = 0; i < maxCount; i++)
			canvas[i] = ' ';

		// player 그림을 canvas 공간에 player 위치에 복사
		for (int i = 0; i < 5; i++)
			canvas[player_pos + i] = player_shape[i];
		player_pos++;

		for (int i = 0; i < 5; i++)
			canvas[enemy_pos + i] = enemy_shape[i];
		// enemy 그림을 canvas 공간에 enemy 위치에 복사
		enemy_pos--;

		canvas[maxCount] = '\0';
		printf("%s\r", canvas);
		Sleep(100);
	}
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
