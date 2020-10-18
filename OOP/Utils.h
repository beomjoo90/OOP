#ifndef UTILS_H_
#define UTILS_H_

#include <Windows.h>

struct Position {
	int x;
	int y;

	Position(int x = 0, int y = 0) : x(x), y(y) {}
	Position(const Position& other) : x(other.x), y(other.y) {} // copy constructor
	
	Position operator+(const Position& other) const
	{
		return Position{ this->x + other.x, this->y + other.y };
	}

	Position operator-(const Position& other) const
	{
		return Position{ this->x - other.x, this->y - other.y };
	}

	Position& operator+=(const Position& source)
	{
		this->x += source.x; this->y += source.y;
		return *this;
	}

	Position& operator=(const Position& source) {
		this->x = source.x; this->y = source.y;
		return *this;
	}

	const Position operator*(int scale) const {
		return Position{ this->x*scale, this->y*scale };
	}

	const Position operator/(int scale) const {
		if (scale == 0) return *this;
		return Position{ (int)(this->x/scale), (int)(this->y/scale) };
	}

	static Position up;
	static Position right;
	static Position down;
	static Position left;
	static Position zeros;
	static Position ones;
};

Position Position::up{ 0, 1 };
Position Position::down{ 0, -1 };
Position Position::right{ 1, 0 };
Position Position::left{ -1, 0 };
Position Position::zeros{ 0, 0 };
Position Position::ones{ 1, 1 };


class Borland {

public:
	static void initialize()
	{
		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
		SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	}

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

#endif 
