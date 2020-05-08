// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>


// commit practice
// 커밋 연습

void printSpaces(int i)
{
	for (int j = 0; j < i; j++)
		printf(" ");
}

void clear(char* canvas, int length)
{
	memset(canvas, ' ', length);
	canvas[length] = '\0';
}

void render(const char* canvas, int lastPosition)
{
	printf("%s\r", canvas);
}

struct GameObject {
	int		pos;
	char	shape[100]; // 0 ... 99

	GameObject(int pos, const char* shape)
		: pos(pos)
	{
		setShape(shape);
	}

	//getter 게터
	int getPos() { return pos; }
	const char* getShape() { return shape; }

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


	bool isInside(int length)
	{
		return pos <= (length - strlen(shape)) && pos >= 0;
	}

	void moveRight()
	{
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	void draw(char* canvas, int maxCount)
	{
		if (isInside(maxCount) == false) return;
		strncpy(&canvas[pos], shape, strlen(shape));
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
		
	void update(int enemy_pos, const char* enemy_shape)
	{
		if (isFired == false) return;

		if (direction == 0)
			moveRight();
		else moveLeft();

		int pos = getPos();
		if ((direction == 0 && enemy_pos <= pos)
			|| (direction == 1 && pos < enemy_pos + strlen(enemy_shape)))
		{
			isFired = false;
		}
		
	}

	void draw(char* canvas, int maxCount)
	{
		if (isFired == false) return;
		GameObject::draw(canvas, maxCount);
	}
}; // 구조체 Bullet 정의

struct Player : public GameObject {
	
	// constructor 생성자
	Player(const char* shape, int maxCount)
		: GameObject(rand() % (maxCount - strlen(shape)), shape)
	{	
	}
	
	void fire(int enemy_pos, Bullet* bullet)
	{
		if (bullet == nullptr) return;
		if (bullet->isFired == true) return;

		bullet->isFired = true;

		int pos = getPos();
		bullet->setPos(pos);
		if (pos < enemy_pos) {
			bullet->setPos( bullet->getPos() + (int)strlen(getShape()) - 1);
			bullet->setShape("-->");
			bullet->direction = 0;
		}
		else {
			bullet->setShape("<--");
			bullet->direction = 1;
		}
	}
};

Bullet* findUnusedBullet(Bullet bullets[], int maxBullets)
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (bullets[i].isFired == true) continue;		
		return &bullets[i];
	}
	return nullptr;
}

int main()
{
	const int maxCount = 80;
	char canvas[maxCount + 1];
	Player	player{ "(o_o)", maxCount };
	Enemy   enemy{ "(*___*)", maxCount };
	Bullet	bullets[maxCount];

	while (true)		
	{
		clear(canvas, maxCount);
				
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
				player.fire(enemy.getPos(), bullet);
				break;
			case 'w':
				enemy.moveRight();
				break;
			case 's':
				enemy.moveLeft();
				break;
			}
		}
		for (int i = 0; i < maxCount; i++)
		{
			if (bullets[i].isFired == false) continue;
			bullets[i].update(enemy.getPos(), enemy.getShape());
		}
		
		// draw game objects to a canvas (player, enemy ...)
		player.draw(canvas, maxCount);
		enemy.draw(canvas, maxCount);
		for (int i = 0; i < maxCount; i++)
		{
			if (bullets[i].isFired == false) continue;
			bullets[i].draw(canvas, maxCount);
		}
		
		// display canvas to a monitor
		render(canvas, maxCount);		
		Sleep(100);
	}
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
