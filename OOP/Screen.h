#pragma once
class Screen
{
	int		len;
	char*	canvas;

public:
	Screen(int maxCount = 80);
	~Screen();

	int length();

	void clear();
	void render();
	void draw(const char* shape, int pos);
};

