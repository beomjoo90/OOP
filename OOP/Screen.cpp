
#include <iostream>
#include "Screen.h"
#include "Utils.h"

using namespace std;

Screen::Screen(int w, int h)
	: width(w), height(h), buffer{ new char[getSize()] }
{
	Borland::initialize();
	buffer[getSize() - 1] = '\0';
}

Screen& Screen::getInstance() {
	static Screen instance{ 80, 30 };
	return instance;
}

Screen::~Screen() { delete[] buffer; }

int Screen::getWidth() { return width; }
int Screen::getScreenWidth() { return width + 1; }
int Screen::getHeight() { return height; }
int Screen::getSize() { return getScreenWidth()*height; }

void Screen::clear() { memset(buffer, ' ', getSize()); buffer[getSize() - 1] = '\0'; }

void Screen::draw(int x, int y, const char shape) { buffer[y* getScreenWidth() + x] = shape; }
void Screen::draw(int x, int y, const char* shape, int len) {
	if (shape == nullptr || len == 0 || len > strlen(shape)) return;
	strncpy(&buffer[y* getScreenWidth() + x], shape, len);
}
void Screen::draw(const Position& pos, char shape) { draw(pos.x, pos.y, shape); }
void Screen::draw(const Position& pos, const char* shape) { draw(pos.x, pos.y, shape, strlen(shape)); }
void Screen::draw(const Position& pos, const char* shape, int len) { draw(pos.x, pos.y, shape, len); }
void Screen::drawLineHorizontal(const Position& pos, int width) {
	if (pos.x < 0 || pos.y < 0 || pos.x + width > getWidth() || pos.y > getHeight()) return;
	for (int i = pos.x; i <= min(this->width, pos.x + width); i++) draw(i, pos.y, 196);
}
void Screen::drawLineVertical(const Position& pos, int height) {
	if (pos.x < 0 || pos.y < 0 || pos.x > getWidth() || pos.y + height > getHeight()) return;
	for (int i = pos.y; i <= min(this->height, pos.y + height); i++) draw(pos.x, i, 179);
}
void Screen::drawRectangle(const Position& topLeft, const Position& sz) {
	drawLineHorizontal(topLeft, sz.x);
	drawLineHorizontal(topLeft + Position{ 0, sz.y }, sz.x);
	drawLineVertical(topLeft, sz.y);
	drawLineVertical(topLeft + Position{ sz.x, 0 }, sz.y);
	draw(topLeft, 218); draw(topLeft + Position{ sz.x, 0 }, 191);
	draw(topLeft + Position{ 0, sz.y }, 192); draw(topLeft + sz, 217);
}
void Screen::drawShape(const Position& pos, const Position& sz, const char* shape)
{
	if (shape == nullptr) return;
	for (int i = 0; i < sz.y; i++) draw(pos.x, pos.y + i, &shape[i*sz.x], sz.x);
}

void Screen::render()
{
	for (int y = 0; y < height - 1; y++)
		draw(width, y, '\n');
	draw(width - 1, height, '\0');

	Borland::gotoxy(0, 0);
	cout << buffer;
}