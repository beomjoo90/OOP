// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Utils.h"

using namespace std;

class Screen {
	int width;
	int height;
	char* canvas;

	static Screen* instance;
	Screen(int width = 70, int height = 25) 
		: width(width), height(height),
		canvas(new char[(width+1)*height])

	{
		Borland::initialize();
	}
public:
	static Screen& getInstance() {
		if (instance == nullptr) {
			instance = new Screen();
		}
		return *instance;
	}

	~Screen() {
		if (instance) {
			delete[] canvas;
			instance = nullptr;
		}
	}

	void draw(const char* shape, int w, int h, const Position& pos)
	{
		if (!shape) return;
		for (int i = 0; i < h; i++)
		{
			strncpy(&canvas[pos.x + (pos.y+i)*(width+1)], &shape[i*w], w);
		}
	}

	void render()
	{
		for (int i = 0; i < height; i++)
			canvas[width + i * (width+1)] = '\n';
		canvas[width + (height-1) * (width + 1)] = '\0';
		Borland::gotoxy(0, 0);
		cout << canvas;
	}

	void clear()
	{
		memset(canvas, ' ', (width + 1)*height);
		canvas[width + (height-1)*(width+1)] = '\0';
	}

};

Screen* Screen::instance = nullptr;

class GameObject {
	char* shape;
	int width;
	int height;
	Position pos;
	int direction;
	Screen& screen;

public:
	GameObject(const char* shape, int width, int height) 
		: height(height), width(width), shape(nullptr), pos(0, 0), direction(1),
		screen(Screen::getInstance())
	{
		if (!shape || strlen(shape) == 0 || width == 0 || height == 0)
		{
			this->shape = new char[1];
			this->shape[0] = 'x';
			width = 1;
			height = 1;
		} else {
			this->shape = new char[width*height];
			strncpy(this->shape, shape, width*height);
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
		screen.draw(shape, width, height, pos);
	}

	void setDirection(int direction) { this->direction = direction; }

	virtual void update(const Position& pos) {
		switch (direction)
		{
		case 1:
		case 2:
			if (this->pos.x < pos.x) {
				(this->pos.x)++;
			}
			else if (this->pos.x > pos.x) {
				(this->pos.x)--;
			}
			break;
		case 3:
		case 4:
			if (this->pos.y < pos.y) {
				(this->pos.y)++;
			}
			else if (this->pos.y > pos.y) {
				(this->pos.y)--;
			}
			break;
		}
		
	}
};

int main()
{
	GameObject player("xxxyyyzzz", 3, 3);
	GameObject enemy2("^^^^^00000^^^^^", 5, 3);
	GameObject enemy("--00--", 2, 3);
	Screen&	 screen = Screen::getInstance();
	INPUT_RECORD InputRecord;
	DWORD Events;

	player.setPos(1, 1);
	enemy.setPos(10, 10);
	enemy2.setPos(5, 5);
	enemy.setDirection(3);	
	enemy2.setDirection(4);

	screen.clear(); screen.render();
	while (true)
	{
		

		player.draw(); enemy.draw(); enemy2.draw();
		screen.render();		
		Sleep(30);
		screen.clear();

		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRecord, 1, &Events);
		if (InputRecord.EventType == MOUSE_EVENT) {
			if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				COORD coord;
				coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
				coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				Position pos;
				pos.x = InputRecord.Event.MouseEvent.dwMousePosition.X;
				pos.y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				player.update(pos);
				enemy.update(pos); enemy2.update(pos);
			}
		}
		
	}

	return 0;
}