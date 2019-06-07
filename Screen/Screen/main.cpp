// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Screen.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

using namespace std;

int main()
{
	Player player = { 30, "(^_^)" };
	Enemy enemy{ 60, "(*--*)" };
	Bullet bullet(-1, "+");
	const int maxGameObjects = 10;
	GameObject* gameObjects[maxGameObjects];
	for (int i = 0; i < maxGameObjects; i++)
		gameObjects[i] = nullptr;
	gameObjects[0] = &player; //upcast
	gameObjects[1] = &enemy;
	gameObjects[2] = &bullet;

	while (true)
	{
		Screen::getInstance().clear();

		if (_kbhit())
		{
			int c = _getch();
			for (int i = 0; i < maxGameObjects; i++)
			{
				GameObject* obj = gameObjects[i];
				if (!obj) continue;
				obj->process_input(c, gameObjects, maxGameObjects);
			}
		}
		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gameObjects[i];
			if (!obj) continue;
			obj->draw();
		}

		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gameObjects[i];
			if (!obj) continue;
			obj->update(gameObjects, maxGameObjects);
		}

		Screen::getInstance().render();
		Sleep(66);
	}

	return 0;
}