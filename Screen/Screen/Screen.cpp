#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Screen.h"


Screen::Screen(int sz) : size(sz), screen(new char[sz + 1])
{
}

Screen::~Screen()
{
	if (screen) {
		delete[] screen;
		screen = nullptr;
	}
}

void Screen::draw(int pos, const char* face)
{
	if (face == nullptr) return;
	if (pos < 0 || pos >= size) return;
	strncpy(&screen[pos], face, strlen(face));
}

void Screen::render()
{
	printf("%s\r", screen);
}

void Screen::clear()
{
	memset(screen, ' ', size);
	screen[size] = '\0';
}

int Screen::length()
{
	return size;
}
