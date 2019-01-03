#include "Controllable.h"

LPDIRECTINPUTDEVICE8 Controllable::Keyboard;

LPDIRECTINPUT8 Controllable::di = NULL;

int Controllable::IsKeyDown(int KeyCode)
{
	return (KeyStates[KeyCode] & 0x80) > 0;
}

void Controllable::OnKeyUp(int KeyCode)
{
}

void Controllable::OnKeyDown(int Keycode)
{
}

void Controllable::_InitKeyboard(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT
		hr = DirectInput8Create
		(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);
	hr = di->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL);
	hr = Keyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = Keyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE);

	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	hr = Keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	hr = Keyboard->Acquire();

}

void Controllable::_ProcessKeyBoard()
{
	// Collect all key states first
	Keyboard->GetDeviceState(sizeof(KeyStates), KeyStates);

	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	HRESULT hr = Keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), KeyEvents, &dwElements, 0);

	// Scan through all data, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = KeyEvents[i].dwOfs;
		int KeyState = KeyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
		{
			OnKeyDown(KeyCode);
		}
		else
			OnKeyUp(KeyCode);
	}
}
