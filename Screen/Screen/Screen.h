#pragma once
class Screen
{
	Screen(int sz);
	int size;
	char* screen;
	static Screen* instance;
public:
	static Screen& getInstance();
	
	~Screen();

	void draw(int pos, const char* face);
	void render();
	void clear();
	int length();
};



