#include "Camera.h"

Camera* Camera::instance;

Camera* Camera::getInstance()
{
	if (instance == nullptr)
		instance = new Camera();
	return instance;
}

void Camera::ToggleFollowCamera()
{
	bFollowCamera = !bFollowCamera;
}

void Camera::UpdateInput(float deltatime)
{
	//_ProcessKeyBoard();
}

void Camera::Update(float deltatime, D3DXVECTOR2 fpos)
{
	if (bFollowCamera)
	{
		position.x = fpos.x - CameraWidth / 2;
		position.y = fpos.y - CameraHeight / 2;
	}
	else if (bFreeMode && !bFollowCamera)
	{
		if (IsKeyDown(DIK_RIGHT))
			position.x += speed;
		if (IsKeyDown(DIK_LEFT))
			position.x -= speed;
		if (IsKeyDown(DIK_DOWN))
			position.y -= speed;
		if (IsKeyDown(DIK_UP))
			position.y += speed;
		//std::cout << "X: " << position.x << " Y: " << position.y << std::endl;
	}
}

void Camera::OnKeyDown(int keycode)
{
	switch (keycode)
	{
		case DIK_ADD:
			speed++;
			break;
		case DIK_SUBTRACT:
			speed--;
			break;
		case DIK_BACKSPACE:
			bFollowCamera = false;
			bFreeMode = true;
			std::cout << "FreeMode true";
			break;
	}
}