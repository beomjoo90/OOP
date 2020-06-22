#pragma once
#include "GameObject.h"
class Screen;

class UI :
	public GameObject
{
	char* data;
	int sz;
	static int currentCursor;
	int count;

public:
	UI(Screen& screen, const char* prefix, int pos, int sz);

	~UI();

	int length() const { return strlen(getShape()) + sz;  }

	void setData(int value);

	int getEndpoint() const { return getPos() + length(); }

	bool isInsideCursor() const { return getPos() <= currentCursor && currentCursor < getEndpoint(); }

	void update();

	void draw();

	void process_input(int key);

	void moveLeft();

	void moveRight();
};

