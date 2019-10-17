// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include "Utils.h"

using namespace std;

class GameObject {
	bool		enabled;

protected:

	Position	pos;
	Screen&		screen;
	char*		shape;
	int			width;
	int			height;

	vector<GameObject *> children;
	GameObject*	parent;

public:
	GameObject(const char* shape, int width, int height, const Position& pos = Position{ 0, 0 })
		: height(height), width(width), shape(nullptr), pos(pos), enabled(true), parent(nullptr),
		screen(Screen::getInstance()) {
		if (!shape || strlen(shape) == 0)
			this->shape = nullptr;
		else {
			this->shape = new char[width*height];
			strncpy(this->shape, shape, width*height);
		}
		children.clear();
	}

	GameObject(int width, int height, const Position& pos = Position{ 0, 0 })
		: GameObject(nullptr, width, height, pos) 
	{}

	virtual ~GameObject()
	{
		while (children.size() > 0) {
			auto back = children.back();
			children.pop_back();
			delete back;
		}
		if (shape) { delete[] shape; }
		shape = nullptr;
		width = 0, height = 0;
	}

	void setParent(GameObject* parent) {
		this->parent = parent;
	}

	void add(GameObject* child) {
		if (!child) return;

		child->setParent(this);
		children.push_back(child);
	}

	virtual bool isActive() { return enabled;  }

	void setActive(bool flag = true) { enabled = flag;  }
		
	void setShape(const char* shape) {
		if (!shape) return;
		strncpy(this->shape, shape, width*height);
	}

	virtual void setPos(int x, int y) { this->pos.x = x; this->pos.y = y; }

	int getWidth() const { return width; }
	int getHeight() const { return height;  }

	Position& getPos() { return pos; }

	vector<GameObject *>& getChildren() {
		return children;
	}
		
	void internalDraw(const Position& inheritedPos = Position{ 0, 0 }) {
		if (!enabled) return;

		draw(inheritedPos);

		for (auto& child : children) child->internalDraw(pos + inheritedPos);
	}

	virtual void draw(const Position& inheritedPos = Position{ 0, 0 }) {
		screen.draw(shape, width, height, pos + inheritedPos);
	}
	
	void internalUpdate() {
		if (!enabled) return;
		update();
		for (auto& child : children) child->internalUpdate();
	}
		
	virtual void update() {}

	static const char rect = '\xB2';
};


class Panel : public GameObject {
	
public:
	Panel(const char* layout, int width, int height, const Position& pos) : GameObject(layout, width, height, pos) {}

	GameObject* pop() {
		auto& children = getChildren();
		auto back = children.back();
		children.pop_back();
		back->setParent(nullptr);
		return back;
	}

	void draw(const Position& inheritedPos) {
		screen.drawRect(Position{ pos.x -1, pos.y -1 } + inheritedPos, getWidth() + 2, getHeight() + 2);
	}
};

class Text : public Panel {

public:
	Text(const char* info, const Position& pos) : Panel(info, strlen(info), 1, pos) {}

	void draw(const Position& inheritedPos) {
		screen.draw(shape, width, 1, pos + inheritedPos);
	}
};

class Score : public Text {
	int score;
	char* buf;

public:
	Score(const Position& pos) : Text("Score:     ", pos), score(0) {
		buf = new char[strlen(shape) + 1];
	}

	~Score() {
		delete[] buf;
	}

	void addScore(int inc) {
		if (inc < 0) return;
		score += inc;
	}

	void draw(const Position& inheritedPos) {
		sprintf(buf, "Score: %3d", score);
		screen.draw(buf, strlen(buf), 1, pos + inheritedPos);
	}
};

class Map : public Panel {
	bool* map;
	int   width;
	int	  height;
	int   upper;
	char  buffer[100];
	Score* score;

public:
	Map(int width, int height, const Position& pos)
		: map(new bool[width*height]), width(width), height(height), upper(height - 1),
		Panel(nullptr, width, height, pos), score(nullptr)
	{
		for (int i = 0; i < width*height; i++) 
			map[i] = false;
	}

	~Map() { if (map) delete[] map; }

	void setScore(Score* score) {
		this->score = score;
	}

	void addScore(int score) {
		if (this->score) this->score->addScore(score);
	}

	bool isLineAllOccupied(int line) {
		for (int i = 0; i < width; i++) {
			if (!map[line*width + i]) {
				return false;
			}
		}
		return true;
	}

	bool evaluateLine(int line) {
		if (!isLineAllOccupied(line)) return false;

		// clean the given line
		for (int i = 0; i < width; i++) map[line*width + i] = false;

		// copy lines above the "line" down below their below lines.
		for (int i = line - 1; i >= (upper-1) && i >= 0; i--) {
			for (int j = 0; j < width; j++) 
				map[(i + 1)*width + j] = map[i*width + j];
			if (i==0) for (int i = 0; i < width; i++) 
				map[line*width + i] = false;
		}
		upper++;
		return true;
	}

	bool isGrounded(const char* shape, const Position& pos, int w, int h) {
		if (pos.y + h >= height) return true;

		// pos.y + h < height
		int next = pos.y + h;
		if (next < upper) return false;
		for (int i = 0; i < h; ++i) {
			for (int j = 0; j < w; j++)
				if (map[pos.x + j + (pos.y + i + 1) * width] && shape[j + i*w] != ' ')
					return true;
		}
		return false;
	}

	bool isOccupied(const Position& pos) {
		return map[pos.x + pos.y *width];
	}

	void place(const char* shape, const Position& pos, int w, int h) {
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (shape[j + i*w] != ' ')
					map[pos.x + j + (pos.y + i)* width] = true;
			}
		}
		if (pos.y < upper) upper = pos.y;
		addScore(1);
	}

	void draw(const Position& inheritedPos)
	{
		screen.drawRect(Position{ pos.x - 1, pos.y -1 } + inheritedPos, width+2, height+2);
		for (int i = upper; i < height; i++) {
			memset(buffer, ' ', 100);
			buffer[width] = '\0';
			for (int j = 0; j < width; j++) {
				if (map[j + i * width]) buffer[j] = GameObject::rect;
			}
			screen.draw(buffer, width, 1, Position{ pos.x, pos.y+ i } + inheritedPos);
		}
	}
};

struct BlockShape {
	string shape;
	int width;
	int height;
};

class Block : public GameObject {
	float x;
	float y;
	float speed;
	bool interactable;
	Map* map;

public:
	Block(const BlockShape& shape, bool interactable = true, const Position& pos = Position{ 0, 0 })
		: GameObject(shape.shape.c_str(), shape.width, shape.height, pos), x(.0f), y(.0f), speed(0.02f), interactable(interactable), map(nullptr)
	{ }

	void rotateShape() {
		static char* shape = new char[GameObject::width*GameObject::height];
		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
				shape[(GameObject::width-1-x)*GameObject::height+y] = this->shape[y*GameObject::width + x];
		setShape(shape);
		swap(GameObject::width, GameObject::height);
	}

	void setInteractable() { interactable = true;  }

	void setPos(int x, int y) {
		GameObject::setPos(x, y);
	}

	void setMap(Map* map) {
		this->map = map;
	}

	void update() {
		static vector<int> comboBonus { 0, 40, 100, 300, 1200 };
		if (isActive() == false) return;
		if (!interactable || !map) return;

		if (map->isGrounded(shape, pos, width, height)) {
			map->place(shape, pos, width, height);
			for (int i = height - 1; i >= 0; --i) {
				int nCombos = 0;
				while (map->evaluateLine(pos.y + i)) {
					nCombos++;
				}
				if (nCombos < comboBonus.size())
					map->addScore( comboBonus[nCombos] );
			}
			setActive(false);
			return;
		}		

		if (Input::GetKeyDown(KeyCode::Right)) {
			x++;
			if (parent) {
				if (x + width >= parent->getWidth()) x = parent->getWidth() - width;
			}
			else {
				if (x + width >= screen.getWidth()/2) x = screen.getWidth() - width;
			}
		}
		if (Input::GetKeyDown(KeyCode::Left)) {
			x--;
			if (x < 0.0f) x = 0.0f;
		}
		if (Input::GetKeyDown(KeyCode::Up)) {
			rotateShape();
		}
		if (Input::GetKeyDown(KeyCode::Down)) {
			speed *= 2;
		}			
		if (Input::GetKeyDown(KeyCode::Space)) {
			if (map) {
				pos.y = y;
				while (!map->isGrounded(shape, pos, width, height)) {
					pos.y++; y+= 1.0f;
				}
				return;
			}
		}
		if (Input::GetKeyDown(KeyCode::A)) {
			rotateShape();
		}
		y += speed;
	}

	void draw(const Position& inheritedPos) {
		
		if (interactable == true) {
			pos.x = (int)x, pos.y = (int)y;
		}

		for (int i = pos.y + 1; map && i < map->getHeight(); i++) {
			if ( map->isOccupied(Position{pos.x, i}))
				break;
			screen.draw("\xFA", 1, 1, Position{ pos.x, i } + inheritedPos);
		}
		for (int i = pos.y + 1; map && i < map->getHeight(); i++) {
			if (map->isOccupied(Position{ pos.x + width - 1, i }))
				break;
			screen.draw("\xFA", 1, 1, Position{ pos.x + width - 1, i } +inheritedPos);
		}
		screen.draw(shape, width, height, pos + inheritedPos);
	}
};

int main()
{
	vector<BlockShape> candidates{
		{ "\xB2\xB2 \xB2 \xB2", 2, 3},
		{ "\xB2\xB2\xB2\xB2",	2, 2},
		{ "\xB2\xB2\xB2\xB2",	4, 1},
		{ "\xB2\xB2\xB2 \xB2 ", 2, 3},
		{ " \xB2\xB2\xB2\xB2 ", 2, 3},
		{ " \xB2\xB2\xB2 \xB2", 2, 3},
		{ "\xB2 \xB2\xB2 \xB2", 2, 3}
	};
	Screen&	 screen = Screen::getInstance();
	vector<GameObject *> gameObjects;

	string mode = "mode con cols=" + to_string(screen.getWidth() + 10);
	mode += " lines=" + to_string(screen.getHeight() + 5);

	std::system(mode.c_str());
	std::system("chcp 437");	

	auto candidate = candidates[rand() % candidates.size()];
	auto next = candidates[rand() % candidates.size()];
		
	auto main = new Map( 25, 40, Position{ 1, 1 });
	auto& children = main->getChildren();	
	main->add(new Block(candidate, true, Position{ rand() % main->getWidth(), 0 }));
	static_cast<Block *>(children[0])->setMap(main);

	auto nextPanel = new Panel(nullptr, screen.getWidth() / 2-2, 10, Position{ screen.getWidth() / 2 + 1, 1 });
	nextPanel->add(new Text("Next", Position{ 2, 4 }));
	auto score = new Score(Position{ 2, 5 });
	nextPanel->add(score);
	nextPanel->add(new Block(next, false, Position{ nextPanel->getWidth()/2 - 2, nextPanel->getHeight()/2-1}) );

	main->setScore(score);

	gameObjects.push_back(main);
	gameObjects.push_back(nextPanel);

	screen.clear(); screen.render();
	
	while (!Input::GetKeyDown(KeyCode::Esc)) {
		screen.clear();
		for (auto obj : gameObjects) obj->internalUpdate();

		bool needANewBlock = false;
		for (auto it = children.begin(); it != children.end(); ) {
			auto child = *it;
			if (child->isActive()) {
				it++;
				continue;
			}
			it = children.erase(it);
			needANewBlock = true;
		}
		if (needANewBlock) {
			auto nextBlock = static_cast<Block *>(nextPanel->pop());
			nextBlock->setInteractable();
			nextBlock->setMap(main);
			nextBlock->setPos(main->getWidth() / 2 - 4, 0);
			main->add(nextBlock);
			next = candidates[rand() % candidates.size()];
			nextPanel->add(new Block(next, false, Position{ nextPanel->getWidth() / 2 - 2, 3 }));
		}
		
		for (auto it = gameObjects.cbegin(); 
			it != gameObjects.cend(); it++)
			(*it)->internalDraw();
				
		screen.render();		
		Sleep(1);		

		Input::EndOfFrame();		
	}
	
	while (gameObjects.size() > 0) {
		auto back = gameObjects.back();
		gameObjects.pop_back();
		delete back;
	}
	
	return 0;
}