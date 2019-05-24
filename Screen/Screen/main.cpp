// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

void draw(char* loc, const char* face)
{
	strncpy(loc, face, strlen(face));
}

class Screen {
	int size;
	char* screen;

public:
	Screen(int sz) : size(sz), screen(new char[sz + 1])
	{}
	~Screen()
	{
		if (screen) {
			delete[] screen;
			screen = nullptr;
		}
	}

	void draw(int pos, const char* face)
	{
		if (face == nullptr) return;
		if (pos < 0 || pos >= size) return;
		strncpy(&screen[pos], face, strlen(face));
	}

	void render()
	{
		printf("%s\r", screen);
	}

	void clear()
	{
		memset(screen, ' ', size);
		screen[size] = '\0';
	}

	int length()
	{
		return size;
	}

};

class GameObject {
	int pos;
	char face[20];
	Screen& screen;

public:
	GameObject(int pos, const char* face, Screen& screen)
		: pos(pos), screen(screen)
	{
		strcpy(this->face, face);
	}

	virtual ~GameObject()
	{}

	int getPosition()
	{
		return pos;
	}

	void setPosition(int pos)
	{
		this->pos = pos;
	}

	virtual void process_input(int input, GameObject* objects[], int maxObjects)
	{
		if (!objects || maxObjects == 0) return;
	}

	virtual void draw()
	{
		screen.draw(pos, face);
	}

	virtual void update(GameObject* objects[], int maxObjects)
	{
		if (!objects || maxObjects == 0) return;
	}

};

class Player : public GameObject {

public:
	Player(int pos, const char* face, Screen& screen)
		: GameObject(pos, face, screen)
	{}

	~Player()
	{}

	void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
	}

	void process_input(int input, GameObject* objects[], int maxObjects)
	{
		if (input == 'a') moveLeft();
		else if (input == 'd') moveRight();
	}

};

class Enemy : public GameObject {

public:
	Enemy(int pos, const char* face, Screen& screen)
		: GameObject(pos, face, screen)
	{}

	~Enemy()
	{}

	void moveRandom()
	{
		setPosition(getPosition() + rand() % 3 - 1);
	}

	void update(GameObject* objects[], int maxObjects)
	{
		if (!objects || maxObjects == 0) return;
		moveRandom();
	}
};

class Bullet : public GameObject {
	bool isFiring;

	void fire(int player_pos)
	{
		isFiring = true;
		setPosition(player_pos);
	}

public:
	Bullet(int pos, const char* face, Screen& screen)
		: GameObject(pos, face, screen), isFiring(false)
	{}

	~Bullet()
	{}

	void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
	}

	void draw()
	{
		if (isFiring == false) return;
		GameObject::draw();
	}

	void process_input(int input, GameObject* objects[], int maxObjects)
	{
		if (input != ' ') return;

		for (int i = 0; i < maxObjects; i++)
		{
			GameObject* obj = objects[i];
			if (!obj) continue;
			Player* player = dynamic_cast<Player*>(obj);
			if (!player) continue;
			fire(player->getPosition());
		}
	}

	void update(GameObject* objects[], int maxObjects)
	{
		if (!objects || maxObjects == 0) return;

		int enemy_pos = -1; // not found any enemy object
		for (int i = 0; i < maxObjects; i++)
		{
			GameObject* obj = objects[i];
			if (!obj) continue;
			Enemy* enemy = dynamic_cast<Enemy*>(obj);
			if (!enemy) continue;
			enemy_pos = enemy->getPosition();
		}
		if (enemy_pos == -1) return; // not found any enemy

		if (isFiring == false) return;
		int pos = getPosition();
		if (pos < enemy_pos) {
			pos = pos + 1;
		}
		else if (pos > enemy_pos) {
			pos = pos - 1;
		}
		else {
			isFiring = false;
		}
		setPosition(pos);
	}
};

#define NORMAL_PLAY 1

#if NORMAL_PLAY
int main()
{
	Screen screen{ 80 };
	Player player = { 30, "(^_^)", screen };
	Enemy enemy{ 60, "(*--*)", screen };
	Bullet bullet(-1, "+", screen);
	const int maxGameObjects = 10;
	GameObject* gameObjects[maxGameObjects];
	for (int i = 0; i < maxGameObjects; i++)
		gameObjects[i] = nullptr;
	gameObjects[0] = &player; //upcast
	gameObjects[1] = &enemy;
	gameObjects[2] = &bullet;

	while (true)
	{
		screen.clear();

		if (_kbhit())
		{
			int c = _getch();
			for (int i = 0; i < maxGameObjects; i++)
			{
				GameObject* obj = gameObjects[i];
				if (!obj) continue;
				obj->process_input(c, gameObjects, maxGameObjects);
			}
		}
		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gameObjects[i];
			if (!obj) continue;
			obj->draw();
		}

		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gameObjects[i];
			if (!obj) continue;
			obj->update(gameObjects, maxGameObjects);
		}

		screen.render();
		Sleep(66);
	}

	return 0;
}
#else

void test()
{
	Screen* screen = nullptr;

	screen = new Screen(80);
	/* 80 };
	Player player = { 30, "(^_^)", &screen };
	Enemy enemy{ 60, "(*--*)", &screen };
	Bullet bullet(-1, "+", &screen);
	*/

}

int main()
{
	test();


	Screen* another = nullptr;
	int ch = _getch();
	return 0;
}

#endif