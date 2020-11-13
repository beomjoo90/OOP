#include <iostream>
#include <Windows.h>
#include <string>
#include "InputManager.h"
#include "Screen.h"

InputManager::InputManager() 
	: hStdin(GetStdHandle(STD_INPUT_HANDLE)), irInBuf{ {0} }
{
	if (hStdin == INVALID_HANDLE_VALUE) return;
	FlushConsoleInputBuffer(hStdin);

	string mode = "mode con:cols=" + to_string(Screen::getInstance().getWidth() + 10);
	mode += " lines=" + to_string(Screen::getInstance().getHeight() + 5);
	std::system(mode.c_str());
	std::system("chcp 437");

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode)) return;
	if (!SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) return;
	events.clear();
}

VOID InputManager::ErrorExit(const char* lpszMessage)
{
	Borland::gotoxy(0, 21);
	printf("%80d\r", ' ');
	printf("%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

VOID InputManager::KeyEventProc(KEY_EVENT_RECORD ker)
{
	if (ker.bKeyDown)
		printf("key pressed\n");
	else
		printf("key released\n");
}

VOID InputManager::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	switch (mer.dwEventFlags)
	{
	case 0:

		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			printf("left button press \n");
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			printf("right button press \n");
		}
		else
		{
			printf("button press\n");
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		printf("mouse moved\n");
		break;
	case MOUSE_WHEELED:
		printf("vertical mouse wheel\n");
		break;
	default:
		printf("unknown\n");
		break;
	}
}

InputManager& InputManager::getInstance() {
	static InputManager instance;
	return instance;
}

bool InputManager::GetKeyDown(WORD ch) {
	if (events.empty() == true) return false;
	const INPUT_RECORD in = events.front();
	if (in.EventType != KEY_EVENT) return false;
	if (in.Event.KeyEvent.bKeyDown == TRUE) {
		return in.Event.KeyEvent.wVirtualKeyCode == ch;
	}
	return false;
}


bool InputManager::GetLeftMouseDown() {
	if (events.empty() == true) return false;
	const INPUT_RECORD in = events.front();
	if (in.EventType != MOUSE_EVENT) return false;
	mousePosition.x = in.Event.MouseEvent.dwMousePosition.X;
	mousePosition.y = in.Event.MouseEvent.dwMousePosition.Y;
	return in.Event.MouseEvent.dwEventFlags == 0
		&& (in.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED);
}

bool InputManager::GetRightMouseDown() {
	if (events.empty() == true) return false;
	const INPUT_RECORD in = events.front();
	if (in.EventType != MOUSE_EVENT) return false;
	mousePosition.x = in.Event.MouseEvent.dwMousePosition.X;
	mousePosition.y = in.Event.MouseEvent.dwMousePosition.Y;
	return in.Event.MouseEvent.dwEventFlags == 0
		&& (in.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED);
}

void InputManager::readInputs() {
	DWORD cNumRead;
	DWORD nEvents;

	if (!GetNumberOfConsoleInputEvents(hStdin, &nEvents)) return;
	if (nEvents == 0) return;

	nEvents = min(nEvents, 128);
	ReadConsoleInput(
		hStdin,      // input buffer handle 
		irInBuf,     // buffer to read into 
		nEvents,         // size of read buffer 
		&cNumRead); // number of records read

	for (int i = 0; i < (int)cNumRead; i++) events.push_back(irInBuf[i]);
}

void InputManager::consumeEvent() {
	if (events.empty() == true) return;
	events.pop_front();
}


