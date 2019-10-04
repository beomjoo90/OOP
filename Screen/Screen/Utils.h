#ifndef UTILS_H_
#define UTILS_H_
#include <cstdio>
#include <Windows.h>

using namespace std;

struct Position {
	int x;
	int y;
	Position(int x = 0, int y = 0) : x(x), y(y) {}

	Position(const Position& other) 
		: Position(other.x, other.y) {}
};

class Input {
	static INPUT_RECORD InputRecord[128];
	static DWORD Events;
	static bool evaluated;
	static bool gotMouseEvent;
	static bool gotKeyEvent;
	static Position mousePosition;
	static WORD vKeyCode;

	static void GetEvent()
	{
		evaluated = true;
		DWORD numEvents = 0;

		GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &numEvents);
		if (!numEvents) return;

		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), InputRecord, numEvents, &Events);
		for (int i = 0; i < Events; i++) {
			if (InputRecord[i].EventType == MOUSE_EVENT) {
				if (InputRecord[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					COORD coord;
					coord.X = InputRecord[i].Event.MouseEvent.dwMousePosition.X;
					coord.Y = InputRecord[i].Event.MouseEvent.dwMousePosition.Y;
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
					mousePosition.x = InputRecord[i].Event.MouseEvent.dwMousePosition.X;
					mousePosition.y = InputRecord[i].Event.MouseEvent.dwMousePosition.Y;
					gotMouseEvent = true;
				}
			}
			else if (InputRecord[i].EventType == KEY_EVENT) {
				if (InputRecord[i].Event.KeyEvent.bKeyDown) {
					vKeyCode = InputRecord[i].Event.KeyEvent.wVirtualKeyCode;
					gotKeyEvent = true;
				}
			}
		}
	}

public:

	static void EndOfFrame()
	{
		evaluated = false;
		gotMouseEvent = false;
		gotKeyEvent = false;
	}

	static void Initialize()
	{

		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT );

		EndOfFrame();
	}
	static bool GetMouseEvent(Position& pos) {
		if (evaluated == false) GetEvent();

		if (gotMouseEvent == true) {
			pos = mousePosition;
			return true;
		}
		return false;}

	static bool GetKeyEvent(WORD& keyCode) {
		if (evaluated == false) GetEvent();

		if (gotKeyEvent == true) {
			keyCode = vKeyCode;
			return true;
		}
		return false;
	}
};
INPUT_RECORD Input::InputRecord[128];
DWORD Input::Events;

bool Input::evaluated = false;
bool Input::gotMouseEvent = false;
bool Input::gotKeyEvent = false;
Position Input::mousePosition{ -1, -1 };
WORD Input::vKeyCode{ 0 };

class Borland {

public:
	

	static int wherex()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}
	static int wherey()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}
	static void gotoxy(int x, int y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}
	static void gotoxy(const Position* pos)
	{
		if (!pos) return;
		gotoxy( (*pos).x, (*pos).y);
	}
	static void gotoxy(const Position& pos)
	{
		gotoxy( pos.x, pos.y);
	}
};

class Screen {
	int width;
	int height;
	char* canvas;

	static Screen* instance;
	Screen(int width = 80, int height = 25)
		: width(width), height(height),
		canvas(new char[(width + 1)*height])

	{
		Input::Initialize();
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
			strncpy(&canvas[pos.x + (pos.y + i)*(width + 1)], &shape[i*w], w);
		}
	}

	void render()
	{
		for (int i = 0; i < height; i++)
			canvas[width + i * (width + 1)] = '\n';
		canvas[width + (height - 1) * (width + 1)] = '\0';
		Borland::gotoxy(0, 0);
		cout << canvas;
	}

	void clear()
	{
		memset(canvas, ' ', (width + 1)*height);
		canvas[width + (height - 1)*(width + 1)] = '\0';
	}


	int getHeight() const { return height;  }
	int getWidth() const { return width;  }
};

Screen* Screen::instance = nullptr;

#endif 
