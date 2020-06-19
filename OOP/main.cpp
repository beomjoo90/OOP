// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib> // include malloc,

#include "Screen.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"


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
				int nActiveBullets = 0;
				for (int i = 0; i < capacity; ++i)
				{
					if (gos[i] == nullptr) continue;
					if (dynamic_cast<Enemy*>(gos[i])) ++nEnemies;
					if (dynamic_cast<Player*>(gos[i])) ++nPlayers;
					if (dynamic_cast<Bullet*>(gos[i])) {
						++nBullets;
						if (static_cast<Bullet*>(gos[i])->checkFire() == true) {
							++nActiveBullets;
						}
					}
				}
				printf("total = %3d, enemies = %3d, players = %3d, bullets = %3d (%3d)\r", capacity, nEnemies, nPlayers, nBullets, nActiveBullets);
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
