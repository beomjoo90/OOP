#pragma once
class Screen
{
	int size;
	char* screen;

public:
	Screen(int sz);
	~Screen();

	void draw(int pos, const char* face);
	void render();
	void clear();
	int length();
};

