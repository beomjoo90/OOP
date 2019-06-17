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
#include "Integer.h"
#include "GameObjects.h"
#include "MyString.h"


using namespace std;

int main()
{
	MyString hello = "hello";
	MyString world = "world";
	MyString helloworld = hello + world;
	world = "new world";
	
	cout << helloworld << hello + world << endl;


#if 0
	Integer i = 10;
	Integer j = 20;
	Integer k( 0 );
	Integer m(-1);
	Integer l(-10);

	l = k = i + j + m;
	//l.operator=(k.operator=(  operator+(operator+(i, j), m) ));

	l.print(); // 29

	Integer::printStat();

	GameObjects& objects = GameObjects::getInstance();

	objects.add(new Player{ 30, "(^_^)" });
	objects.add(new Enemy{ 60, "(*--*)" });

	Screen& screen = Screen::getInstance();
	while (true)
	{
		screen.clear();

		objects.update();

		screen.render();
		Sleep(66);
	}
#endif
	return 0;
}