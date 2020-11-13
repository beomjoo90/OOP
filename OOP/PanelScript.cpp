#include <iostream>
#include "Utils.h"
#include "GameObject.h"
#include "PanelScript.h"

using namespace std;


PanelScript::PanelScript(GameObject* gameObject)
	: Component(gameObject), count(0)
{
}
void PanelScript::start() {
}

void PanelScript::update() {
	Borland::gotoxy(0, 33);
	count++;
	cout << "panel script of " << gameObject->getName() << ", " << count;

}


