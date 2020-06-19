#pragma once

class GameObjectManager;
class Screen;

class GameObject
{
	int		pos;
	char	shape[100]; // 0 ... 99
	static  GameObjectManager gameObjectManager;
	Screen& screen;

public:
	GameObject(Screen& screen, int pos, const char* shape);
	virtual ~GameObject();

	//getter 게터
	int getPos() const;
	const char* getShape() const;
	Screen& getScreen() const;

	static GameObject** getGameObjects();
	static int getMaxGameObjects();

	//setter 세터
	void setPos(int pos);
	void setShape(const char* shape);

	bool isInside() const;
	void moveRight();
	void moveLeft();

	virtual void process_input(int key);
	virtual void update();
	virtual void draw();
};

