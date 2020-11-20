#include "Transform.h"
#include "GameObject.h"
#include "Screen.h"

void Transform::update() {
	Borland::gotoxy(0, 32);
	std::cout << "transform of " << gameObject->getName() << ", " << count++;
}

void Transform::draw() {
	(Screen::getInstance()).drawShape(pos, size, shape.c_str() );
}
