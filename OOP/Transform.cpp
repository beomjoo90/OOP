#include "Transform.h"
#include "GameObject.h"

void Transform::update() {
	Borland::gotoxy(0, 32);
	std::cout << "transform of " << gameObject->getName() << ", " << count++;
}
