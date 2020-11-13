#pragma once
#include "Utils.h"

using namespace std;

class Screen {
	int width;
	int height;
	char* buffer;

	Screen(int w = 10, int h = 10);
	static Screen* instance;

public:

	static Screen& getInstance();

	~Screen();

	int getWidth();
	int getScreenWidth();
	int getHeight();
	int getSize();

	void clear();

	void draw(int x, int y, const char shape);
	void draw(int x, int y, const char* shape, int len);
	void draw(const Position& pos, char shape);
	void draw(const Position& pos, const char* shape);
	void draw(const Position& pos, const char* shape, int len);
	void drawLineHorizontal(const Position& pos, int width);
	void drawLineVertical(const Position& pos, int height);
	void drawRectangle(const Position& topLeft, const Position& sz);
	void drawShape(const Position& pos, const Position& sz, const char* shape);
	void render();
};
