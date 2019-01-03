#include "DirectX.h"

// Khai báo các biến toàn cục
#pragma region Define

#pragma endregion

#pragma region Initialize
//Khởi tạo các giá trị ban đầu
bool DirectX::Initialize(HWND hwnd)
{
	if (!DirectX_Initialize(hwnd))
	{
		std::cout << "DirectX_Initialize fail!"<< endl;
		return false;
	}
	if (!Game_Initialize())
	{
		std::cout << "Game_Initialize fail!" << endl;
		return false;
	}
	return true;
}

//Khởi tạo các giá trị ban đầu cần cho DirectX
bool DirectX::DirectX_Initialize(HWND hwnd)
{
	//Initialize Game Device
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddv);

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &back_buffer);

	if (d3ddv == NULL)
	{
		MessageBox(NULL, TEXT("Game Init False!"), TEXT("ERROR!"), MB_OK);
		return false;
	}

	d3ddv->CreateOffscreenPlainSurface(100, 100, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);

	//Lấy biến chứa thông số DirectX cho Sprite
	Sprite::GetDevice(d3ddv);
	Sprite::Initialize();
	
	//Debug
	//Sprite::PrintSpritePos(2);
	//cout << "Call From DirectX.cpp Line 54" << endl;
	//Sprite::PrintAnimationClip(2);

	return true;
}

//Khởi tạo các giá trị ban đầu của Game
bool DirectX::Game_Initialize()
{
	gameScene.Initialize();
	return true;
}

#pragma endregion

#pragma region GameLoop

// Hàm túm hết mấy cái vòng lặp game bỏ vào đây
void DirectX::GameLoop(float deltatime)
{
	UpdateInput(deltatime);
	//Collision::getInstance()->BoxUpdater();
	Collision::getInstance()->CollisionChecker(deltatime);
	Update(deltatime);
	this->DrawLoop();
}

//Vòng lặp nhận sự kiện từ người chơi
void DirectX::UpdateInput(float deltatime)
{
	gameScene.UpdateInput(deltatime);
}

//Vòng lặp tính toán logic trong game
void DirectX::Update(float deltatime)
{
	gameScene.Update(deltatime);
}

//Vòng lặp để vẽ
void DirectX::DrawLoop()
{
	d3ddv->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	if (d3ddv->BeginScene())
	{
		d3ddv->ColorFill(surface, NULL, D3DCOLOR_XRGB(255, 0, 0));
		//Viết code ở đây
		gameScene.Draw();

		d3ddv->EndScene();
	}

	d3ddv->Present(NULL, NULL, NULL, NULL);
}

#pragma endregion

bool DirectX::GameRelease()
{
	return true;
}
