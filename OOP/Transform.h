#pragma once
#include <iostream>
#include "Component.h"
#include "Utils.h"

using namespace std;

class GameObject;

class Transform :
	public Component
{
	Position pos;
	string shape;
	Position size;
	int count;
public:
	Transform(GameObject* gameObject)		
		: Component(gameObject), count(0),
		pos{ 0,0}, shape(""), size{ 1,1 }
	{}

	Position getPosition() const { return pos; }
	void setPosition(const Position& pos) { this->pos = pos; }
	void setPosition(int x, int y) { this->pos.x = x; this->pos.y = y; }

	void setShape(const string& shape) { this->shape = shape; }
	string getShape() const { return shape; }

	void setSize(const Position& size) {
		auto shapeLen = shape.size();
		if (size.x * size.y > shapeLen) return;
		this->size = size;
	}
	Position getSize() const { return size; }

	void update() override;

	void draw() override;
};

