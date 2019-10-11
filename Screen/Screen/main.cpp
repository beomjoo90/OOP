// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <ctime>
#include "Utils.h"

using namespace std;

class GameObject {

	bool active;
	

protected:
	char*		shape;
	int			width;
	int			height;
	Position	pos;
	Screen&		screen;
	vector<GameObject *> children;

public:
	GameObject(const char* shape, int width, int height, const Position& pos = Position{ 0, 0 } )
		: height(height), width(width), 
		shape(nullptr), pos(pos), active(true),
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

		while (children.size()) {
			auto back = children.back();
			children.pop_back();
			delete back;
		}

	}

	void setActive(bool active) {
		this->active = active;
	}

	bool isActive() const { return active;  }

	void add(GameObject* child) {
		if (!child) return;
		children.push_back(child);
	}

	void setShape(const char* shape) {
		if (!shape) return;
		strncpy(this->shape, shape, width*height);
	}

	void setPos(int x, int y) { this->pos.x = x; this->pos.y = y; }

	Position& getPos() { return pos; }

	void internalDraw(const Position& accumulatedPos = Position{ 0,0 })
	{
		if (active == false) return;
		draw(accumulatedPos);
		for (auto child : children) 
			child->internalDraw( pos + accumulatedPos );
	}
		
	virtual void draw(const Position& accumulatedPos = Position{ 0,0 })
	{		
		screen.draw(shape, width, height, pos + accumulatedPos);
	}

	void internalUpdate()
	{
		if (active == false) return;
		update();
		for (auto child : children)
			child->internalUpdate();
	}

	virtual void update() 
	{
		
	}

	vector<GameObject *>& getChildren() { return children;  }	
};

class Block : public GameObject {	

	vector<string> sprites;
	int current;
	bool movable;

public:
	Block(vector<string>& sprites, int w, int h, bool movable=true,
		const Position& pos = Position{ 0,0 } )
		: sprites(sprites), current(0), movable(movable),
		GameObject(sprites[current].c_str(), w, h, pos) {}

	void update() {
		if (movable == false) return;

		if (Input::GetKeyDown(KeyCode::Right)) {
			getPos().x++;
		}
		if (Input::GetKeyDown(KeyCode::Left)) {
			getPos().x--;
		}
		if (Input::GetKeyDown(KeyCode::Up)) {
			current = (current + 1) % sprites.size();
			setShape(sprites[current].c_str()); 
		}
		if (Input::GetKeyDown(KeyCode::Up)) {
			current = (current + 1) % sprites.size();
			setShape(sprites[current].c_str());
		}
		if (Input::GetKeyDown(KeyCode::Up)) {
			getPos().y = Screen::getInstance().getHeight();
		}		
		if (Input::GetKeyDown(KeyCode::A)) {
			current = (current + 1) % sprites.size();
			setShape(sprites[current].c_str());
		}
		if (Input::GetKeyDown(KeyCode::D)) {
			current = (current + 3) % sprites.size();
			setShape(sprites[current].c_str());
		}
		
		pos.y++;
		if (pos.y >= screen.getHeight()-1) {
			setActive(false);
		}
		
		
	}
};

int main()
{
	vector<string> sprites { "\xB1\xB1  \xB1  \xB1 ",
		"  \xB1\xB1\xB1\xB1   ", 
		" \xB1  \xB1  \xB1\xB1", 
		"   \xB1\xB1\xB1\xB1  "
	};
	vector<string> sprites2{ "\xB2\xB2  ", "  \xB2\xB2" };
	
	Screen&	 screen = Screen::getInstance();
	vector<GameObject *> gameObjects;

	srand(time(nullptr));

	string mode{ "mode con cols="
		+ to_string(screen.getWidth() + 4)
		+ " lines=" + to_string(screen.getHeight() + 5) };
	system(mode.c_str());
	system("chcp 437");

	auto parent = new Block{ sprites, 3,3, true,
		Position{screen.getWidth()/2, 0} };
	

	auto child = new Block{ sprites2, 2,2, false, Position{5, 0} };
	parent->add(child);
	gameObjects.push_back(parent);	
		
	screen.clear(); screen.render();		

	while (true)
	{	
		screen.clear();

		bool erased = false;
		for (auto it = gameObjects.begin(); it != gameObjects.end(); )
		{
			auto obj = *it;
			if (obj->isActive() == false) {
				it = gameObjects.erase(it);
				erased = true;
				delete obj;
				
			}
			else {
				it++;
			}
		}
		if (erased == true) {
			gameObjects.push_back( new Block{ sprites2, 2,2, true, Position{screen.getWidth() / 2, 0} } );
		}


		for (auto obj : gameObjects) obj->internalUpdate();

		for (auto it = gameObjects.cbegin(); 
			it != gameObjects.cend(); it++)
			(*it)->internalDraw();
		
		screen.render();		
		Sleep(150);

		Input::EndOfFrame();		
	}

	return 0;
}