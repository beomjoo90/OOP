#ifndef UTILS_H_
#define UTILS_H_
#include <cstdio>
#include <Windows.h>

using namespace std;

struct Position {
	int x;
	int y;
	Position(int x = 0, int y = 0) : x(x), y(y) {}
};

class Input {
	static INPUT_RECORD InputRecord;
	static DWORD Events;
	static bool evaluated;
	static bool gotEvent;
	static Position mousePosition;

public:

	static void EndOfFrame()
	{
		evaluated = false;
		gotEvent = false;
	}

	static void Initialize()
	{

		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	}
	static bool GetMouseEvent(Position& pos) {

		if (evaluated == true) {
			if (gotEvent == true) {
				pos = mousePosition;
				return true;
			}
			return false;
		}
		evaluated = true;
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &InputRecord, 1, &Events);
		if (InputRecord.EventType == MOUSE_EVENT) {
			if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				COORD coord;
				coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
				coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				mousePosition.x = InputRecord.Event.MouseEvent.dwMousePosition.X;
				mousePosition.y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
				gotEvent = true;
			}
		}
		return gotEvent;
	}
};
INPUT_RECORD Input::InputRecord;
DWORD Input::Events;

bool Input::evaluated = false;
bool Input::gotEvent = false;
Position Input::mousePosition{ -1, -1 };

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
	Screen(int width = 70, int height = 25)
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

};

Screen* Screen::instance = nullptr;

#endif 
