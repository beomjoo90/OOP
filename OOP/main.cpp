// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib> 
#include <deque>
#include <string>
#include <vector>
#include "Utils.h"

using namespace std;

// forward declaration

class Screen {
	int width;
	int height;
	char* buffer;

	Screen(int w = 10, int h = 10)
		: width(w), height(h), buffer{ new char[getSize()] }
	{	
		Borland::initialize();
		buffer[getSize() - 1] = '\0';
	}
	static Screen* instance;

public:

	static Screen* getInstance() {
		if (instance == nullptr) {
			instance = new Screen{ 80, 20 };
		}
		return instance;
	}

	~Screen() { delete[] buffer; }

	int getWidth() const { return width; }
	int getScreenWidth() const { return width + 1; }
	int getHeight() const { return height; }
	int getSize() const { return getScreenWidth()*height; }

	void clear() { memset(buffer, ' ', getSize()); buffer[getSize() - 1] = '\0';  }

	void draw(int x, int y, const char shape) { buffer[y* getScreenWidth() + x] = shape; }
	void draw(int x, int y, const char* shape) {
		if (shape == nullptr) return;
		strncpy(&buffer[y* getScreenWidth() + x], shape, strlen(shape));
	}
	void draw(const Position& pos, char shape) { draw(pos.x, pos.y, shape); }
	void draw(const Position& pos, const char* shape) { draw(pos.x, pos.y, shape); }
	void drawLineHorizontal(const Position& pos, int width) {
		if (pos.x < 0 || pos.y < 0 || pos.x + width > getWidth() || pos.y > getHeight()) return;
		for (int i = pos.x; i <= min(this->width, pos.x + width); i++) draw(i, pos.y, 178);
	}
	void drawLineVertical(const Position& pos, int height) {
		if (pos.x < 0 || pos.y < 0 || pos.x > getWidth() || pos.y + height > getHeight()) return;
		for (int i = pos.y; i <= min(this->height,  pos.y + height); i++) draw(pos.x, i, 178);
	}
	void drawRectangle(const Position& topLeft, const Position& sz) {
		drawLineHorizontal(topLeft, sz.x);
		drawLineHorizontal(topLeft + Position{ 0, sz.y }, sz.x);
		drawLineVertical(topLeft, sz.y);
		drawLineVertical(topLeft + Position{ sz.x, 0 }, sz.y);
	}	

	void render()
	{
		for (int y = 0; y < height - 1; y++) 
			draw(width, y, '\n');
		draw(width - 1, height, '\0');

		Borland::gotoxy(0, 0);
		cout << buffer;
	}
};

Screen* Screen::instance = nullptr;

// 싱글톤
// 클래스로 생성된 인스턴스가 하나만 존재하도록 만드는 것.
// 생성자 함수를 딱 한번 부르는 것
// 싱글폰 패턴
// 0. 생성자 함수를 private하게 선언
// 1. 외부에 노출된 getInstance라는 static 함수를 사용하도록 강제한다.(변수의 주소 반환)
// 2. 동적 생성된 객체 인스턴스를 저장할 instance 변수를 private하게 선언

// 사용하는 이유
// 만약 사용하지 않는다면 아예 인스턴스를 생성하지 않는다.
// 런타임에 초기화된다.
// 싱글턴을 상속할 수 있다.


// singleton
class InputManager {

	INPUT_RECORD irInBuf[128];

	HANDLE hStdin;
	DWORD fdwSaveOldMode;

	deque<INPUT_RECORD> events;

	InputManager() : hStdin(GetStdHandle(STD_INPUT_HANDLE)), irInBuf{ {0} }	{
		if (hStdin == INVALID_HANDLE_VALUE) return;
		FlushConsoleInputBuffer(hStdin);

		string mode = "mode con:cols=" + to_string(Screen::getInstance()->getWidth() + 10);
		mode += " lines=" + to_string(Screen::getInstance()->getHeight() + 5);
		std::system(mode.c_str());
		std::system("chcp 437");

		if (!GetConsoleMode(hStdin, &fdwSaveOldMode)) return;		
		if (!SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) return;
		events.clear();
	}

	VOID ErrorExit(const char* lpszMessage)
	{
		Borland::gotoxy(0, 21);
		printf("%80d\r", ' ');
		printf("%s\n", lpszMessage);

		// Restore input mode on exit.

		SetConsoleMode(hStdin, fdwSaveOldMode);

		ExitProcess(0);
	}

	VOID KeyEventProc(KEY_EVENT_RECORD ker)
	{
		if (ker.bKeyDown)
			printf("key pressed\n");
		else 
			printf("key released\n");
	}

	VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
	{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
		Borland::gotoxy(0, 22);
		printf("%80d\r", ' ');

		printf("Mouse event: %d %d      ", mer.dwMousePosition.X, mer.dwMousePosition.Y);

		switch (mer.dwEventFlags)
		{
		case 0:

			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				printf("left button press \n");
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				printf("right button press \n");
			}
			else
			{
				printf("button press\n");
			}
			break;
		case DOUBLE_CLICK:
			printf("double click\n");
			break;
		case MOUSE_HWHEELED:
			printf("horizontal mouse wheel\n");
			break;
		case MOUSE_MOVED:
			printf("mouse moved\n");
			break;
		case MOUSE_WHEELED:
			printf("vertical mouse wheel\n");
			break;
		default:
			printf("unknown\n");
			break;
		}
	}

	static InputManager* instance;
public:
	static InputManager* getInstance() {
		if (instance == nullptr) {
			instance = new InputManager;
		}
		return instance;
	}

	bool GetKeyDown(WORD ch) {
		if (events.empty() == true) return false;
		const INPUT_RECORD& in = events.front();
		if (in.EventType != KEY_EVENT) return false;
		if (in.Event.KeyEvent.bKeyDown == TRUE) {
			return in.Event.KeyEvent.wVirtualKeyCode == ch;
		}
		return false;
	}

	bool GetLeftMouseDown() {
		if (events.empty() == true) return false;
		const INPUT_RECORD& in = events.front();
		if (in.EventType != MOUSE_EVENT) return false;
		return in.Event.MouseEvent.dwEventFlags == 0
			&& (in.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED);
	}

	bool GetRightMouseDown() {
		if (events.empty() == true) return false;
		const INPUT_RECORD& in = events.front();
		if (in.EventType != MOUSE_EVENT) return false;
		return in.Event.MouseEvent.dwEventFlags == 0
			&& (in.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED);
	}

	void readInputs() {
		DWORD cNumRead;
		DWORD nEvents;

		if (!GetNumberOfConsoleInputEvents(hStdin, &nEvents)) return;
		if (nEvents == 0) return;

		nEvents = min(nEvents, 128);
		ReadConsoleInput(
			hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			nEvents,         // size of read buffer 
			&cNumRead); // number of records read

		for (int i = 0; i < (int)cNumRead; i++) events.push_back(irInBuf[i]);
	}

	void consumeEvent()
	{
		if (events.empty() == true) return;
		events.pop_front();
	}
};

class GameObject {

	string shape;
	Position pos;

	Position parentPos; // your parent's global position in space
	bool dirty; // mark it TRUE if your local position is changed.

protected:
	Screen& screen;
	InputManager& inputManager;

	vector<GameObject *> children;
	GameObject* parent;

	void setParentPos(const Position& parentPos) { this->parentPos = parentPos; }	

public:
	GameObject(int x, int y, const string& shape, GameObject* parent = nullptr)
		: shape(shape), screen(*Screen::getInstance()),
		inputManager(*InputManager::getInstance()),
		pos(x, y), parent(parent), dirty(false)
	{
		setParentPos(parent ? parent->getWorldPos() : Position::zeros);
		if (parent) parent->add(this);
		
		start();
	}

	GameObject(const Position& pos, const string& shape, GameObject* parent = nullptr) 
		: GameObject(pos.x, pos.y, shape, parent) {}

	virtual ~GameObject() {}

	void add(GameObject* child) {
		if (!child) return;
		children.push_back(child);
	}

	void setPos(int x, int y) { setPos(Position{ x, y }); }
	void setPos(const Position& pos) { 
		this->pos.x = pos.x; this->pos.y = pos.y;
		dirty = true;
	}

	Position getPos() const { return pos; }
	Position getWorldPos() const { return parentPos + pos; }

	void setParent(GameObject* parent) {
		this->parent = parent;
		setParentPos(parent ? parent->getWorldPos() : Position::zeros);
		for (auto child : children) child->internalUpdatePos(true);
	}

	virtual void start() {}

	void internalUpdatePos(bool dirty = false) {
		bool inheritedDirty = dirty;
		if (inheritedDirty == false) {
			if (this->dirty == true) inheritedDirty = true;
		} else {
			if (parent) setParentPos(parent->getWorldPos());
		}
		
		for (auto child : children) child->internalUpdatePos(inheritedDirty);
		this->dirty = false;
	}

	void internalUpdate() {
		update();
		for (auto child : children) child->internalUpdate();
	}
	virtual void update() {}

	void internalDraw() { 
		draw();
		for (auto child : children) child->internalDraw();
	}
	virtual void draw() { screen.draw(getWorldPos(), shape.c_str()); }
};

class Block : public GameObject {

public:
	Block(const Position& pos, const string& shape, GameObject* parent = nullptr)
		: GameObject(pos, shape, parent) {}

	void update() override
	{
		Position pos = getPos();
		if (inputManager.GetKeyDown(VK_DIVIDE)) setPos(pos + Position::left);
		if (inputManager.GetKeyDown(VK_MULTIPLY)) setPos(pos + Position::right);
	}
};

class Panel : public GameObject {
	int width;
	int height;
	
public:
	Panel(const Position& pos, int width, int height, GameObject* parent) : GameObject(pos, "", parent), 
		width(width), height(height)
	{}

	void update() override
	{
		Position pos = getPos();
		if (inputManager.GetKeyDown(VK_LEFT)) setPos(pos + Position::left);
		if (inputManager.GetKeyDown(VK_RIGHT)) setPos(pos + Position::right);
		if (inputManager.GetKeyDown(VK_UP)) setPos(pos + Position::down);
		if (inputManager.GetKeyDown(VK_DOWN)) setPos(pos + Position::up);
	}

	void draw() override
	{	
		Position pos = getWorldPos();
		screen.drawRectangle(Position{ pos.x - 1, pos.y - 1 }, Position{ width + 1, height + 1 });
	}
};

class Text : public GameObject {

public:
	Text(const Position& pos, const string& message, GameObject* parent)
		: GameObject(pos, message.c_str(), parent)
	{}

	void update() override
	{
		Position pos = getPos();
		if (inputManager.GetLeftMouseDown()) setPos(pos + Position::left);
		if (inputManager.GetRightMouseDown()) setPos(pos + Position::right);
	}	
};

InputManager* InputManager::instance = nullptr;

int main()
{
	Screen& screen = *Screen::getInstance();
	InputManager& inputManager = *InputManager::getInstance();
	vector<GameObject*> scene;	

	auto panel = new Panel{ Position{3,3}, 10, 10, nullptr };
	new Text{ Position{0,3}, "Hello", new Block{ Position::zeros, "(^_^)", panel } };

	auto panel2 = new Panel{ Position{20, 3}, 10, 10, nullptr };
	new Text{ Position{0,3}, "World",  new Block{ Position::zeros, "(+_+)", panel2 } };

	scene.push_back(panel);
	scene.push_back(panel2);

	while (true)
	{
		screen.clear();

		inputManager.readInputs();

		if (inputManager.GetKeyDown(VK_ESCAPE)) break;

		for (auto object : scene) object->internalUpdate();
		for (auto object : scene) object->internalUpdatePos(false);
		for (auto object : scene) object->internalDraw();

		screen.render();

		inputManager.consumeEvent();
		
		Sleep(100);
	}
	printf("\nTerminated Successfully.\n");

	return 0;
}

