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

class Screen {
	int		len;
	char*	canvas;

public:
	Screen(int maxCount = 80) 
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

class Enemy; // forward declaration

class GameObject {
	int		pos;
	char	shape[100]; // 0 ... 99

public:
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

	virtual void update(Enemy* enemy) {}

	virtual void draw(Screen& screen)
	{
		if (isInside(screen.length()) == false) return;
		screen.draw(shape, pos);
	}
};

class Enemy : public GameObject {
	
public:
	Enemy(const char* shape, int maxCount)
		: GameObject(rand() % (maxCount - (int)strlen(shape)), shape )
	{}
	~Enemy() {
		int a = 10;
	}
};

class BlinkableEnemy : public Enemy {
	bool isBlinking;
	int count;

public:
	BlinkableEnemy(const char* shape, int maxCount)
		: Enemy(shape, maxCount), isBlinking(false), count(0)
	{}

	void setBlinking() { 
		isBlinking = true;
		count = 10;
	}

	// overriding
	void update(Enemy* enemy)
	{
		if (isBlinking == true) {
			count--;
			if (count == 0) {
				isBlinking = false;
			}
		}
	}

	// overriding
	void draw(Screen& screen)
	{
		if (isBlinking == false) {
			GameObject::draw(screen);
			return;
		}
		if (count % 2 == 0) GameObject::draw(screen);
	}

};

class Bullet : public GameObject {
	bool	isFired;
	int		direction;
			
public:
	Bullet(const char* shape = "")
		: GameObject(-1, shape), isFired(false), direction(0)
	{
	}

	~Bullet() {}

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
	void update(Enemy* enemy)
	{
		if (checkFire() == false) return;

		if (isDirectionRight())
			moveRight();
		else moveLeft();

		int pos = getPos();
		int enemy_pos = enemy->getPos();
		const char* enemy_shape = enemy->getShape();
		if ((isDirectionRight() && enemy_pos <= pos)
			|| (!isDirectionRight() && pos <= enemy_pos + strlen(enemy_shape)))
		{
			if (isDirectionRight() && enemy_pos == pos ||
				!isDirectionRight() && pos == enemy_pos + strlen(enemy_shape)) {
				BlinkableEnemy* be = dynamic_cast<BlinkableEnemy *>(enemy);
				if (be != nullptr) be->setBlinking();
			}
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

class Player : public GameObject {
	
public:
	// constructor 생성자
	Player(const char* shape, int maxCount)
		: GameObject(rand() % (maxCount - strlen(shape)), shape)
	{	
	}

	~Player() {}
	
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
	Player* player = new Player{ "(o_o)", maxCount };
	Enemy* enemy = new BlinkableEnemy{ "(*_______*)", maxCount };
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
	gos[maxCount] = player;
	gos[maxCount + 1] = enemy;
	
	bool requestExit = false;
	while (requestExit == false)		
	{
		screen.clear();
		
		// update game objects (player, enemy ...)
		if (player->isInside(maxCount) == false || enemy->isInside(maxCount) == false)
			break; // check game loop termination condition

		if (_kbhit()) {
			int key = _getch();
			Bullet* bullet = nullptr;

			//printf("\n%c %d\n", key, key);
			switch (key) {
			case 'a':
				player->moveLeft();
				break;
			case 'd':
				player->moveRight();
				break;
			case ' ':
				bullet = findUnusedBullet(bullets, maxCount);
				if (bullet == nullptr) break;				
				player->fire(enemy->getPos(), *bullet);
				break;
			case 'w':
				enemy->moveRight();
				break;
			case 's':
				enemy->moveLeft();
				break;
			case 'z':
				requestExit = true;
				break;
			}
		}
		for (int i = 0; i < maxCount + 2; i++)
		{
			gos[i]->update(enemy);
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

	for (int i = 0; i < maxCount + 2; i++)
	{
		if (gos[i] != nullptr)
			delete gos[i];
		gos[i] = nullptr;
	}
	free((void *)bullets);
	free((void *)gos);
	return 0;
}
