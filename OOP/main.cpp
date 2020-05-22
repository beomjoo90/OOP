// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib> // include malloc,


// commit practice
// 커밋 연습

struct Screen {
	int		len;
	char*	canvas;

	Screen(int maxCount = 95) 
		: len(maxCount+1), canvas{ (char*) malloc(sizeof(char)*(maxCount+1))}
	{
	}

	// 소멸자, destructor
	~Screen()
	{
		free((void *)canvas);
		canvas = nullptr;
		len = 0;
	}

	int length() { return len-1; }

	void clear()
	{
		memset(canvas, ' ', len-1);
		canvas[len-1] = '\0';
	}

	void render()
	{
		printf("%s\r", canvas);
	}

	void draw(const char* shape, int pos)
	{
		strncpy(&canvas[pos], shape, strlen(shape));
	}

};

struct GameObject {
	int		pos;
	char	shape[100]; // 0 ... 99

	GameObject(int pos, const char* shape)
		: pos(pos)
	{
		setShape(shape);
	}

	virtual ~GameObject() {} // 가상 소멸자 함수

	//getter 게터
	int getPos() const { return pos; }
	const char* getShape() const { return shape; }
	
	//setter 세터
	void setPos(int pos) { this->pos = pos;  }
	void setShape(const char* shape) 
	{
		int len = strlen(shape); // len >= 0
		if (len >= 100) {
			// 0 ... 98
			// 99에는 널문자
			strncpy(this->shape, shape, 100 - 1);
			this->shape[100 - 1] = '\0';
		}
		else
			strcpy(this->shape, shape);
	}


	bool isInside(int length) const
	{
		return pos <= (length - strlen(shape)) && pos >= 0;
	}

	void moveRight() {
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	virtual void update(int enemy_pos, const char* enemy_shape) {}

	virtual void draw(Screen& screen)
	{
		if (isInside(screen.length()) == false) return;
		screen.draw(shape, pos);
	}
};

struct Enemy : public GameObject {
	
	Enemy(const char* shape, int maxCount)
		: GameObject(rand() % (maxCount - (int)strlen(shape)), shape )
	{	
	}
};

struct Bullet : public GameObject {
	bool	isFired;
	int		direction;
			
	Bullet(const char* shape = "")
		: GameObject(-1, shape), isFired(false), direction(0)
	{
	}

	bool checkFire() 
	{
		return isFired == true;
	}
	void setFire()
	{
		isFired = true;
	}
	void resetFire()
	{
		isFired = false;
	}

	void makeDirectionLeft() { direction = 1; }
	void makeDirectionRight() { direction = 0; }
	bool isDirectionRight() { return direction == 0; }

	//overriding
	void update(int enemy_pos, const char* enemy_shape)
	{
		if (checkFire() == false) return;

		if (isDirectionRight())
			moveRight();
		else moveLeft();

		int pos = getPos();
		if ((isDirectionRight() && enemy_pos <= pos)
			|| (!isDirectionRight() && pos < enemy_pos + strlen(enemy_shape)))
		{
			resetFire();
		}
		
	}

	//overriding
	void draw(Screen& screen)
	{
		if (checkFire() == false) return;
		GameObject::draw(screen);
	}
}; // 구조체 Bullet 정의

struct Player : public GameObject {
	
	// constructor 생성자
	Player(const char* shape, int maxCount)
		: GameObject(rand() % (maxCount - strlen(shape)), shape)
	{	
	}
	
	void fire(int enemy_pos, Bullet& bullet)
	{
		if (bullet.checkFire()) return;
		bullet.setFire();
		int pos = getPos();
		bullet.setPos(pos);
		if (pos < enemy_pos) {
			bullet.setPos( bullet.getPos() + (int)strlen(getShape()) - 1);
			bullet.setShape("-->");
			bullet.makeDirectionRight();
		}
		else {
			bullet.setShape("<--");
			bullet.makeDirectionLeft();
		}
	}
};

Bullet* findUnusedBullet(Bullet** bullets, int maxBullets)
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (bullets[i]->checkFire()) continue;		
		return bullets[i];
	}
	return nullptr;
}

int main()
{
	Screen screen;
	int maxCount = screen.length();
	Player	player{ "(o_o)", maxCount };
	Enemy   enemy{ "(*___*)", maxCount };
	Bullet** bullets = (Bullet**)malloc(sizeof(Bullet*)*maxCount);
	for (int i = 0; i < maxCount; i++)
	{
		bullets[i] = new Bullet();
	}

	GameObject** gos = (GameObject**)malloc(sizeof(GameObject*)*(maxCount + 2));
	for (int i = 0; i < maxCount; i++)
	{
		gos[i] = bullets[i];
	}
	gos[maxCount] = &player;
	gos[maxCount + 1] = &enemy;
	
	while (true)		
	{
		screen.clear();
		
		// update game objects (player, enemy ...)
		if (player.isInside(maxCount) == false || enemy.isInside(maxCount) == false)
			break; // check game loop termination condition

		if (_kbhit()) {
			int key = _getch();
			Bullet* bullet = nullptr;

			//printf("\n%c %d\n", key, key);
			switch (key) {
			case 'a':
				player.moveLeft();
				break;
			case 'd':
				player.moveRight();
				break;
			case ' ':
				bullet = findUnusedBullet(bullets, maxCount);
				if (bullet == nullptr) break;				
				player.fire(enemy.getPos(), *bullet);
				break;
			case 'w':
				enemy.moveRight();
				break;
			case 's':
				enemy.moveLeft();
				break;
			}
		}
		for (int i = 0; i < maxCount + 2; i++)
		{
			gos[i]->update(enemy.getPos(), enemy.getShape());
		}


		for (int i = 0; i < maxCount + 2; i++)
		{
			gos[i]->draw(screen);
		}
		
		
		// display canvas to a monitor
		screen.render();
		Sleep(100);
	}
	printf("\n정상적으로 종료되었습니다.\n");

	for (int i = 0; i < maxCount; i++)
	{
		if (bullets[i] != nullptr)
			delete bullets[i];
		bullets[i] = nullptr;
	}
	free((void *)bullets);
	return 0;
}
