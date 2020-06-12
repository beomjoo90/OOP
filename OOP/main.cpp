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
class GameObject; // forward declaration

class GameObjectManager {
	GameObject** gos;
	int          capacity;

public:
	GameObjectManager(int capacity = 20) 
		: capacity{ capacity }, gos{ (GameObject**)malloc(sizeof(GameObject*)*capacity) }
	{
		for (int i = 0; i < capacity; i++)
			gos[i] = (GameObject*)nullptr;
	}
	~GameObjectManager() {
	
		for (int i = 0; i < capacity; i++)
		{
			if (gos[i] == nullptr) continue;
			delete gos[i];
			gos[i] = (GameObject*)nullptr;
		}
		free(gos);
		gos = (GameObject**)nullptr;
		capacity = 0;
	}

	void add(GameObject* obj)
	{
		if (obj == nullptr) return;
		// obj != nullptr
		for (int i = 0; i < capacity; i++)
		{
			if (gos[i] != nullptr) continue;
			// gos[i] == nullptr
			gos[i] = obj;
			return;
		}
		// i == capacity
		gos = (GameObject**)realloc(gos, sizeof(GameObject*) * 2 * capacity);
		for (int i = capacity; i < 2 * capacity; i++)
		{
			gos[i] = nullptr;
		}
		gos[capacity] = obj;
		capacity *= 2;
	}

	void remove(GameObject* obj)
	{
		if (obj == nullptr) return;
		for (int i = 0; i < capacity; i++)
		{
			if (gos[i] == obj) {
				gos[i] = nullptr;
				return;
			}
		}
	}

	GameObject** getGameObjects() { return gos; }
	int getCapacity() { return capacity; }
};

class GameObject {
	int		pos;
	char	shape[100]; // 0 ... 99
	static  GameObjectManager gameObjectManager;
	Screen& screen;

public:
	GameObject(Screen& screen, int pos, const char* shape)
		: screen(screen), pos(pos)
	{
		setShape(shape);
		gameObjectManager.add(this);
	}

	virtual ~GameObject() {} // 가상 소멸자 함수

	//getter 게터
	int getPos() const { return pos; }
	const char* getShape() const { return shape; }
	Screen& getScreen() const { return screen;  }

	static GameObject** getGameObjects() { return gameObjectManager.getGameObjects(); }
	static int getMaxGameObjects() { return gameObjectManager.getCapacity(); }
	
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


	bool isInside() const
	{
		return pos <= (screen.length() - strlen(shape)) && pos >= 0;
	}

	void moveRight() {
		pos++;
	}

	void moveLeft()
	{
		pos--;
	}

	virtual void process_input(int key) {}

	virtual void update() {}

	virtual void draw()
	{
		if (isInside() == false) return;
		screen.draw(shape, pos);
	}
};

class Enemy : public GameObject {
	
public:
	Enemy(Screen& screen, const char* shape)
		: GameObject(screen, rand() % (screen.length() - (int)strlen(shape)), shape )
	{}
	~Enemy() {}

	// overriding : 재정의
	void process_input(int key)
	{
		switch (key) {
		case 'w': moveRight(); break;
		case 's': moveLeft(); break;
		}
	}
};

class BlinkableEnemy : public Enemy {
	bool isBlinking;
	int count;

public:
	BlinkableEnemy(Screen& screen, const char* shape)
		: Enemy(screen, shape), isBlinking(false), count(0)
	{}

	void setBlinking() { 
		isBlinking = true;
		count = 10;
	}

	// overriding : 재정의
	void process_input(int key)
	{
		switch (key) {
		case 'j': moveLeft(); break;
		case 'k': moveRight(); break;
		}
	}

	// overriding
	void update()
	{
		if (isBlinking == true) {
			count--;
			if (count == 0) {
				isBlinking = false;
			}
		}
	}

	// overriding
	void draw()
	{
		if (isBlinking == false) {
			GameObject::draw();
			return;
		}
		if (count % 2 == 0) GameObject::draw();
	}

};

class Bullet : public GameObject {
	bool	isFired;
	int		direction;
			
public:
	Bullet(Screen& screen, const char* shape = "")
		: GameObject(screen, -1, shape), isFired(false), direction(0)
	{}

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
	void update()
	{
		if (checkFire() == false) return;

		if (isDirectionRight())
			moveRight();
		else moveLeft();

		// find enemy
		Enemy* enemy = nullptr;
		// find enemy from GameObjectArray (gos)

		GameObject** gos = getGameObjects();
		int maxGameObjects = getMaxGameObjects();
		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gos[i];
			if (obj == nullptr) continue;
			enemy = dynamic_cast<Enemy*>(obj);
			if (enemy != nullptr) break;
		}
		// enemy == nullptr || enemy != nullptr
		if (enemy == nullptr) return;

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
	void draw()
	{
		if (checkFire() == false) return;
		GameObject::draw();
	}
}; // 구조체 Bullet 정의

class Player : public GameObject {
	
	Bullet* findUnusedBullet()
	{
		GameObject** gos = getGameObjects();
		int maxGameObjects = getMaxGameObjects();
		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gos[i];
			if (obj == nullptr) continue;

			// obj != nullptr
			Bullet* bullet = dynamic_cast<Bullet*>(obj);
			if (bullet == nullptr) continue;

			// bullet != nullptr
			if (bullet->checkFire() == true) continue;

			// bullet != nullptr && bullet->checkFire() == false
			return bullet;
		}
		return nullptr;
	}

	void fire()
	{
		Bullet* bullet = findUnusedBullet();
		if (bullet == nullptr) {
			bullet = new Bullet{ getScreen() } ;
		}
		// bullet != nullptr

		// bullet != nullptr && bullet->checkFire() == false
		Enemy* enemy = nullptr;
		// find enemy
		GameObject** gos = getGameObjects();
		int maxGameObjects = getMaxGameObjects();
		for (int i = 0; i < maxGameObjects; i++)
		{
			GameObject* obj = gos[i];
			if (obj == nullptr) continue;
			enemy = dynamic_cast<Enemy*>(obj);
			if (enemy != nullptr) break;
			
		}
		if (enemy == nullptr) return;
		//enemy != nullptr
		int enemy_pos = enemy->getPos();
		bullet->setFire();
		int pos = getPos();
		bullet->setPos(pos);
		if (pos < enemy_pos) {
			bullet->setPos(bullet->getPos() + (int)strlen(getShape()) - 1);
			bullet->setShape("-->");
			bullet->makeDirectionRight();
		}
		else {
			bullet->setShape("<--");
			bullet->makeDirectionLeft();
		}
	}

public:
	// constructor 생성자
	Player(Screen& screen, const char* shape)
		: GameObject(screen, rand() % (screen.length() - strlen(shape)), shape)
	{}

	~Player() {}

	// overriding
	void process_input(int key)
	{
		Bullet* bullet = nullptr;

		switch (key) {
		case 'a': moveLeft(); break;
		case 'd': moveRight(); break;
		case ' ':
			fire();
			break;
		}
	}
	
	
};

GameObjectManager GameObject::gameObjectManager{1};

int main()
{
	Screen screen{ 80 };

	new BlinkableEnemy{ screen, "(*_*)" };
	new Enemy{ screen, "(+_+)" };
	new Player{ screen, "(o_o)" };
	
	bool requestExit = false;
	while (requestExit == false)		
	{
		screen.clear();
		GameObject** gos = GameObject::getGameObjects();
		int capacity = GameObject::getMaxGameObjects();

		// update game objects (player, enemy ...)
		// gos, maxGameObjects
		for (int i = 0; i < capacity; i++)
		{	
			GameObject* obj = gos[i];
			if (obj == nullptr) continue;			

			// obj != nullptr
			// search player
			Player* player = dynamic_cast<Player *>(obj); // dynamically downcast			
			if (player != nullptr) {
				// if player exists, check whether it is inside screen. otherwise, exit.
				if (player->isInside() == false) {
					requestExit = true;
					break;
				}
				continue;
			}
		}

		if (_kbhit()) {
			int key = _getch();
			if (key == 'z') {
				break; // exit from main loop
			}
			if (key == 'i') {
				int nEnemies = 0;
				int nPlayers = 0;
				int nBullets = 0;
				for (int i = 0; i < capacity; i++)
				{
					if (gos[i] == nullptr) continue;
					if (dynamic_cast<Enemy*>(gos[i])) nEnemies++;
					if (dynamic_cast<Player*>(gos[i])) nPlayers++;
					if (dynamic_cast<Bullet*>(gos[i])) nBullets++;
				}
				printf("total = %3d, enemies = %3d, players = %3d, bullets = %3d\r", capacity, nEnemies, nPlayers, nBullets);
				Sleep(3000);
				continue;
			}

			for (int i = 0; i < capacity; i++)
			{
				if (gos[i] == nullptr) continue;
				gos[i]->process_input(key);
			}
		}
		gos = GameObject::getGameObjects();
		capacity = GameObject::getMaxGameObjects();
		for (int i = 0; i < capacity; i++)
		{
			if (gos[i] == nullptr) continue;
			gos[i]->update();
		}


		for (int i = 0; i < capacity; i++)
		{
			if (gos[i] == nullptr) continue;
			gos[i]->draw();
		}
		
		
		// display canvas to a monitor
		screen.render();
		Sleep(100);
	}
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
