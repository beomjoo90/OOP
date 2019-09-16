// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Utils.h"

using namespace std;

class GameObject {
	char* shape;
	int width;
	int height;
	Position pos;
	int direction;

public:
	GameObject(const char* shape, int width, int height) : height(height), width(width), shape(nullptr), pos(0, 0), direction(1)
	{
		if (!shape || strlen(shape) == 0 || width == 0 || height == 0)
		{
			this->shape = new char[1];
			this->shape[0] = 'x';
			width = 1;
			height = 1;
		} else {
			this->shape = new char[(width + 1)*height];
			for (int i = 0; i < height; i++) {
				strncpy(&this->shape[i*(width + 1)], &shape[i*(width + 1)], width);
				this->shape[i*(width + 1) + width] = '\0';
			}
		}
		this->width = width;
		this->height = height;
	}
	virtual ~GameObject() {
		if (shape) { delete[] shape; }
		width = 0, height = 0;
	}

	void setPos(int x, int y) { this->pos.x = x; this->pos.y = y; }

	void draw() {
		Borland::gotoxy(pos);
		for (int i = 0; i < height; i++) {
			cout << &shape[i*(width + 1)];
			Borland::gotoxy(pos.x, pos.y + i+1);
		}
	}

	void erase() {
		Borland::gotoxy(pos);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				cout << " ";
			}
			Borland::gotoxy(pos.x, pos.y + i+1);
		}
	}

	void setDirection(int direction) { this->direction = direction; }

	virtual void update() {
		switch (direction)
		{
		case 1:
			(this->pos).x++;
			if (this->pos.x > 70) {
				this->pos.x = 70;
				this->direction = 2;
			}
			break;

		case 2:
			(this->pos).x--;
			if (this->pos.x < 0) {
				this->pos.x = 0;
				this->direction = 1;
			}
			break;

		case 3:
			(this->pos).y++;
			if (this->pos.y > 20) {
				this->pos.y = 20;
				this->direction = 4;
			}
			break;

		case 4:
			(this->pos).y--;
			if (this->pos.y < 0) {
				this->pos.y = 0;
				this->direction = 3;
			}
			break;
		}
		
	}
};

int main()
{
	GameObject player("xxx\nyyy\nzzz\n", 3, 3);
	GameObject enemy("--\n00\n--\n", 2, 3);
	player.setPos(1, 1);
	enemy.setPos(10, 10);
	enemy.setDirection(3);

	while (true)
	{
		player.draw(); enemy.draw();
		Sleep(30);
		player.erase(); enemy.erase();

		player.update(); enemy.update();
	}

	return 0;
}