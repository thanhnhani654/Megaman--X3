#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Sprite.h"
#include "GameScene.h"
#include "Component\Collision.h"

class DirectX
{
private:
	LPDIRECT3D9 d3d = nullptr;
	LPDIRECT3DDEVICE9 d3ddv;
	LPDIRECT3DSURFACE9 surface = NULL;
	LPDIRECT3DSURFACE9 back_buffer = NULL;

	bool DirectX_Initialize(HWND hwnd);
	bool Game_Initialize();
	void UpdateInput(float deltatime);
	//void CollisionUpdate(float deltatime);
	//void CollisionProcess(float deltatime);
	void Update(float deltatime);
	void DrawLoop();

	GameScene gameScene;
public:
	bool Initialize(HWND hwnd);
	void GameLoop(float deltatime);
	bool GameRelease();
};