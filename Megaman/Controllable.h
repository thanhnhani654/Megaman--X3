#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define KEY_DOWN(code) ( IsKeyDown(code) )
#define KEYBOARD_BUFFER_SIZE	1024

class Controllable
{
private:
	LPDIRECTINPUT8 dinput = NULL;
	LPDIRECTINPUTDEVICE8 didev = NULL;
	static LPDIRECTINPUTDEVICE8 Keyboard;
	BYTE KeyStates[256];
	DIDEVICEOBJECTDATA KeyEvents[KEYBOARD_BUFFER_SIZE];
	static LPDIRECTINPUT8 di;
public:

	static void _InitKeyboard(HWND hWnd, HINSTANCE hInstance);

	void _ProcessKeyBoard();

	int IsKeyDown(int KeyCode);

	virtual void OnKeyUp(int Keycode);
	virtual void OnKeyDown(int Keycode);

};