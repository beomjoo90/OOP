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
	char*		shape;
	int			width;
	int			height;
	Position	pos;
	int			direction;
	Screen&		screen;

public:
	GameObject(const char* shape, int width, int height) 
		: height(height), width(width), shape(nullptr), pos(0, 0), direction(1),
		screen(Screen::getInstance()) {
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

	void setShape(const char* shape) {
		if (!shape) return;
		strncpy(this->shape, shape, width*height);
	}

	void setPos(int x, int y) { this->pos.x = x; this->pos.y = y; }

	Position& getPos() { return pos; }
		
	void draw() {
		screen.draw(shape, width, height, pos);
	}

	void setDirection(int direction) { this->direction = direction; }

	virtual void update() {}
};

class Player : public GameObject {	

	static const string block[4];
	int current;

public:
	Player() : current(0), GameObject(block[current].c_str(), 3, 3) {
		setPos(10, 1);
	}

	void update() {
		WORD keyCode;

		if (Input::GetKeyEvent(keyCode)) {
			switch (keyCode) {
			case VK_RIGHT:
				getPos().x++;
				break;

			case VK_LEFT:
				getPos().x--;
				break;

			case VK_UP:
				current = (current + 1) % 4;
				setShape(block[current].c_str());
				break;
				break;

			case VK_DOWN:
				getPos().y = 70;
				break;

			case 0x41: //'a'
				current = (current + 1) % 4;
				setShape(block[current].c_str());
				break;

			case 0x44: //'d'
				current = (current + 3) % 4;
				setShape(block[current].c_str());
				break;
			}
		}
		getPos().y = (getPos().y + 1)% 80;
	}
};
const string Player::block[4]{ "\xB1\xB1  \xB1  \xB1 ", "  \xB1\xB1\xB1\xB1   ", " \xB1  \xB1  \xB1\xB1", "   \xB1\xB1\xB1\xB1  " };

int main()
{
	Screen&	 screen = Screen::getInstance();
	vector<GameObject *> gameObjects;

	system("mode con cols=45 lines=85");
	system("chcp 437");

	gameObjects.push_back(new Player);
		
	screen.clear(); screen.render();		

	while (true)
	{	
		screen.clear();
		for (auto obj : gameObjects) obj->update();

		for (auto it = gameObjects.cbegin(); 
			it != gameObjects.cend(); it++)
			(*it)->draw();
		
		screen.render();		
		Sleep(150);

		Input::EndOfFrame();		
	}

	return 0;
}