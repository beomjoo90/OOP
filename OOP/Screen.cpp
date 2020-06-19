#include <iostream>
#include "Screen.h"

Screen::Screen(int maxCount)
	: len(maxCount + 1), canvas{ (char*)malloc(sizeof(char)*(maxCount + 1)) }
{
}

// ¼Ò¸êÀÚ, destructor
Screen::~Screen()
{
	free((void *)canvas);
	canvas = nullptr;
	len = 0;
}

int Screen::length() { return len - 1; }

void Screen::clear()
{
	memset(canvas, ' ', len - 1);
	canvas[len - 1] = '\0';
}

void Screen::render()
{
	printf("%s\r", canvas);
}

void Screen::draw(const char* shape, int pos)
{
	strncpy(&canvas[pos], shape, strlen(shape));
}