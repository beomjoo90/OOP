// main.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib> 
#include "Utils.h"

using namespace std;

// forward declaration

class Screen {
	char* buffer;
	int width;
	int height;

	Screen(int width = 10, int height = 10)
		: width(width), height(height), buffer(new char[getSize()])
	{
		Borland::initialize();
		buffer[getSize() - 1] = '\0';
	}
	static Screen* instance;

public:

	static Screen* getInstance() {
		if (instance == nullptr) {
			instance = new Screen{ 10,10 };
		}
		return instance;
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

Screen* Screen::instance = nullptr;

// 싱글톤
// 클래스로 생성된 인스턴스가 하나만 존재하도록 만드는 것.
// 생성자 함수를 딱 한번 부르는 것
// 싱글폰 패턴
// 0. 생성자 함수를 private하게 선언
// 1. 외부에 노출된 getInstance라는 static 함수를 사용하도록 강제한다.(변수의 주소 반환)
// 2. 동적 생성된 객체 인스턴스를 저장할 instance 변수를 private하게 선언

// 사용하는 이유
// 만약 사용하지 않는다면 아예 인스턴스를 생성하지 않는다.
// 런타임에 초기화된다.
// 싱글턴을 상속할 수 있다.


int main()
{
	Screen* screen = Screen::getInstance();
	

	bool requestExit = false;
	int x = 0, y = 0;

	while (requestExit == false)
	{
		screen->clear();
		screen->draw(x, y, '0' + x);
		screen->render();

		// debugging
		Borland::gotoxy(20, 20);
		printf("x = %d, y = %d", x, y);

		Sleep(100);
		
		++x %= screen->getWidth();
		++y %= screen->getHeight();
	}
	printf("\n정상적으로 종료되었습니다.\n");
	return 0;
}
