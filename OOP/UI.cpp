#include <iostream>
#include "UI.h"
#include "Screen.h"

int UI::currentCursor = 25;


UI::UI(Screen& screen, const char* prefix, int pos, int sz)
	: GameObject(screen, pos, prefix), sz(sz), data{ (char *) malloc(strlen(prefix) + sz +1)}
{}

UI::~UI() 
{
	free(data);
}

void UI::setData(int value)
{
	static char buf[100];

	sprintf(buf, "%s%d", getShape(), value);
	strncpy(data, buf, length());
}

void UI::draw()
{
	if (isInsideCursor() == false) {
		getScreen().draw(data, getPos());
		return;
	}
	if (count % 2) {
		getScreen().draw(data, getPos());
	}
}

void UI::process_input(int key)
{
	switch (key)
	{
	case 'v': moveLeft(); break;
	case 'n': moveRight(); break;
	}
}

void UI::moveLeft()
{
	if (isInsideCursor() == false) return;

	GameObject** gos = GameObject::getGameObjects();
	int capacity = GameObject::getMaxGameObjects();
	int myPos = getPos();
	int closest = getScreen().length();
	for (int i = 0; i < capacity; i++)
	{
		if (gos[i] == nullptr || gos[i] == this) continue;
		UI* ui = dynamic_cast<UI *>(gos[i]);
		if (ui == nullptr) continue;
		int dist = myPos - ui->getPos();
		if (dist < 0) continue;
		if (dist < closest) {
			closest = dist;
		}
	}
	if (closest == getScreen().length()) return;
	currentCursor = myPos - closest;
}

void UI::moveRight()
{

}

void UI::update()
{
	if (isInsideCursor() == false) return;

	count++;
}

