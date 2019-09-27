// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include "Utils.h"

using namespace std;

class GameObject {
	char* shape;
	int width;
	int height;
	Position pos;
	int direction;
	Screen& screen;
	Position mousePosition;
public:
	GameObject(const char* shape, int width, int height) 
		: height(height), width(width), shape(nullptr), pos(0, 0), direction(1), mousePosition{ -1,-1 },
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

	virtual void update() {

		
		if (Input::GetMouseEvent(mousePosition) == false) return;

		switch (direction)
		{
		case 1:
		case 2:
			if (this->pos.x < mousePosition.x) {
				(this->pos.x)++;
			}
			else if (this->pos.x > mousePosition.x) {
				(this->pos.x)--;
			}
			break;
		case 3:
		case 4:
			if (this->pos.y < mousePosition.y) {
				(this->pos.y)++;
			}
			else if (this->pos.y > mousePosition.y) {
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

	int a;

	vector<GameObject *> gameObjects;

	gameObjects.push_back(&player);
	gameObjects.push_back(&enemy);
	gameObjects.push_back(&enemy2);
	
	player.setPos(1, 1);
	enemy.setPos(10, 10);
	enemy2.setPos(5, 5);
	enemy.setDirection(3);	
	enemy2.setDirection(4);

	screen.clear(); screen.render();
	while (true)
	{	
		for (auto it = gameObjects.cbegin(); 
					it != gameObjects.cend(); it++)
			(*it)->draw();

		screen.render();		
		Sleep(30);
		screen.clear();

		for (auto obj : gameObjects) obj->update();
		

		Input::EndOfFrame();
		
	}

	return 0;
}