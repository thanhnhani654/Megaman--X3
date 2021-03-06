#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "../Controllable.h"

#define CameraWidth 250
#define CameraHeight 250
#define PRINT_POSITION FALSE

enum eCamerastage
{
	khong,
	mot,
	hai,
	haichamnam,
	ba,
	bon,
	nam,
	sau,
	bay,
	tam,
	chin,
	HornetFight
};

class Camera : public Controllable
{
private:
	D3DXVECTOR2 position;
	static Camera* instance;
	bool bFollowCamera;
	bool bFreeMode;
	bool bStageCamera;
	float speed;
	D3DXVECTOR2 size;
	
	bool lockX;
	bool lockY;
	float limitRightX;
	float limitLeftX;
	float limitTopY;
	float limitBottomY;

public:
	eCamerastage stage;


	static Camera* getInstance();
	Camera()
	{
		bFollowCamera = true;
		bFreeMode = false;
		bStageCamera = true;
		stage = eCamerastage::khong;
		position.x = 0;
		position.y = 0;
		speed = 2;
		size.x = CameraWidth;
		size.y = CameraHeight;

		lockX = false;
		lockY = true;
		

	}

	float GetSpeed() { return speed; }

	D3DXVECTOR2 GetPosition()
	{
		return position;
	}

	D3DXVECTOR2 GetSize()
	{
		return size;
	}

	void SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	void ToggleFollowCamera();

	bool IsFollowCamera();

	void Update(float deltatime, D3DXVECTOR2 fpos);
	void UpdateCameraStage(float deltatime, D3DXVECTOR2 fpos);

	void UpdateInput(float deltatime);

	void OnKeyDown(int keycode);
};