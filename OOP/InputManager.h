#pragma once

#include <Windows.h>
#include <deque>
#include "Utils.h"

using namespace std;



class InputManager{

	INPUT_RECORD irInBuf[128];

	HANDLE hStdin;
	DWORD fdwSaveOldMode;

	deque<INPUT_RECORD> events;

	InputManager();
	VOID ErrorExit(const char* lpszMessage);
	VOID KeyEventProc(KEY_EVENT_RECORD ker);

	VOID MouseEventProc(MOUSE_EVENT_RECORD mer);

public:
	static InputManager& getInstance();

	bool GetKeyDown(WORD ch);

	Position mousePosition; // it will be used only when a mouse event occurs.

	bool GetLeftMouseDown();
	bool GetRightMouseDown();

	void readInputs();
	void consumeEvent();
};

