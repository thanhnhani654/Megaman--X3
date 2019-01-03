#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "../Controllable.h"

#define CameraWidth 400
#define CameraHeight 400

class Camera : public Controllable
{
private:
	D3DXVECTOR2 position;
	static Camera* instance;
	bool bFollowCamera;
	bool bFreeMode;
	float speed;
	D3DXVECTOR2 size;
public:
	static Camera* getInstance();
	Camera()
	{
		bFollowCamera = true;
		bFreeMode = false;
		position.x = 0;
		position.y = 0;
		speed = 2;
		size.x = CameraWidth;
		size.y = CameraHeight;
	}

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

	void Update(float deltatime, D3DXVECTOR2 fpos);

	void UpdateInput(float deltatime);

	void OnKeyDown(int keycode);
};