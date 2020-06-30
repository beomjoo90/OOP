#pragma once

class GameObjectManager;
class Screen;

class GameObject
{
	int		pos;
	char	shape[100]; // 0 ... 99
	static  GameObjectManager gameObjectManager;
	Screen& screen;
	bool    active;

public:
	GameObject(Screen& screen, int pos, const char* shape);
	virtual ~GameObject();

	//getter 게터
	int getPos() const { return pos; }
	const char* getShape() const { return shape; }
	Screen& getScreen() const { return screen; }
	bool isActive() const { return active;  }

	bool isColliding(const GameObject* opponent) const;

	void setActive(bool active = true) { this->active = active;  }
	static GameObject** getGameObjects();
	static int getMaxGameObjects();

	//setter 세터
	void setPos(int pos) { this->pos = pos; }
	void setShape(const char* shape);

	virtual void OnDamage(GameObject* collider) {}
	
	bool isInside() const;
	
	virtual void moveRight() { ++pos; }
	virtual void moveLeft() { --pos; }

	virtual void process_input(int key) {}
	virtual void update() {}
	virtual void draw();
};

