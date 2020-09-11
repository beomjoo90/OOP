// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib> 
#include "Utils.h"

using namespace std;

class Screen {
	char* buffer;
	int width;
	int height;

public:
	Screen(int width = 10, int height = 10)
		: width(width), height(height), buffer(new char[getSize()])
	{
		Borland::initialize();
		buffer[getSize()-1] = '\0';
	}

	~Screen() { delete[] buffer; }

	int getWidth() const { return width; }
	int getScreenWidth() const { return width + 1; }
	int getHeight() const { return height; }
	int getSize() const { return getScreenWidth()*height; }

	void clear() { memset(buffer, ' ', getSize()); buffer[getSize() - 1] = '\0';  }

	void draw(int x, int y, char shape) { buffer[y* getScreenWidth() + x] = shape; }

	void render()
	{
		for (int y = 0; y < height - 1; y++) {
			buffer[y * getScreenWidth() + width] = '\n';
		}
		buffer[getSize()-1] = '\0';

		Borland::gotoxy(0, 0);
		cout << buffer;
	}
};


int main()
{
	Screen screen{ 10, 10 };

	bool requestExit = false;
	int x = 0, y = 0;

	while (requestExit == false)
	{
		screen.clear();
		screen.draw(x, y, '0' + x);
		screen.render();

		// debugging
		Borland::gotoxy(20, 20);
		printf("x = %d, y = %d", x, y);

		Sleep(100);
		
		++x %= screen.getWidth();
		++y %= screen.getHeight();
	}
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
