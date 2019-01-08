#include <iostream>
#include <Windows.h>
#include "DirectX.h"
#include <time.h>
#include "Controllable.h"

#define BSHOWFPS TRUE

using namespace std;

int GameLoop(DirectX& directX);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WinAPI_Initialize(HWND &hwnd, HINSTANCE hInstance);
void ShowFPS(bool show, float deltatime);
float GetDeltaTime();

int winHeight = 250;
int winWidth  = 250;

int main()
{
	char title[500];  // to hold title
	GetConsoleTitleA(title, 500);
	HWND hwnd = FindWindowA(NULL, title);
	HWND WinAPIHwnd;
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
	DirectX directX;
	
	WinAPI_Initialize(hwnd, hInstance);
	directX.Initialize(hwnd);
	Controllable::_InitKeyboard(hwnd, hInstance);

	MSG msg;
	int done = 0;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = 1;
			TranslateMessage(&msg);
			DispatchMessage(&msg);			
		}

		GameLoop(directX);
	}

	directX.GameRelease();

	return msg.wParam;

	system("pause");
}

int GameLoop(DirectX& directX)
{
	//Tính DeltaTime
	float deltatime = GetDeltaTime();
	ShowFPS(BSHOWFPS, deltatime);

	//limitFPS
	/*if (deltatime < 0.0333)
		Sleep((0.0333 - deltatime)*1000);*/
	//if (deltatime < 0.025)
	//	Sleep((0.025 - deltatime) * 1000);
	

	// Chạy game
	directX.GameLoop(deltatime);
	return 1;
}


int WinAPI_Initialize(HWND &hwnd, HINSTANCE hInstance)
{		
	WNDCLASS wc;

	//wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("BattleCity");
	//wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("CANT RUN"), TEXT("BattleCity"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindow(//WS_EX_CLIENTEDGE,
		wc.lpszClassName,
		TEXT("Client"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		winWidth,
		winHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void ShowFPS(bool show, float deltatime)
{
	if (!show)
		return;

	static float time;
	static int countFPS;
	time += deltatime;
	if (time >= 1)
	{
		cout << countFPS << "\t" << deltatime << endl;
		time = 0;
		countFPS = 0;
	}
	countFPS++;
}

float GetDeltaTime()
{
	static clock_t currentTime = clock();
	static clock_t lastTime = clock();

	currentTime = clock();
	float deltatime = currentTime - lastTime;
	lastTime = clock();

	return deltatime / 1000;
}