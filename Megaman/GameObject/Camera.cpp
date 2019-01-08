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
	if (PRINT_POSITION)
		std::cout << "Camera X: " << position.x << "\tCamera Y: " << position.y << std::endl;
	if (bFollowCamera)
	{
		if (!bStageCamera)
		{
			position.x = fpos.x - CameraWidth / 2;
			position.y = fpos.y - CameraHeight / 2;
		}
		else
		{
			UpdateCameraStage(deltatime, fpos);
		}
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

void Camera::UpdateCameraStage(float deltatime, D3DXVECTOR2 fpos)
{
	switch (stage)
	{
	case khong:
		position.x = 200;
		position.y = 2960;
		limitLeftX = 30;
		limitRightX = 775;
		stage = eCamerastage::mot;
		break;
	case mot:
		if (position.x >= limitLeftX)
			position.x = fpos.x - CameraWidth / 2;
		else
			position.x = limitLeftX;

		if (position.x > limitRightX)
		{
			stage = eCamerastage::hai;
			limitBottomY = 2960;
			limitTopY = 3470;
		}
		break;
	case hai:
		if (position.y >= limitBottomY && fpos.y - CameraHeight / 2 >= limitBottomY)
			position.y = fpos.y - CameraHeight / 2;
		else
			position.y = limitBottomY;
		if (position.y > limitTopY)
		{
			stage = eCamerastage::ba;
			limitLeftX = 1000;
			limitRightX = 2280 - CameraWidth;
			limitBottomY = 2685;
			limitTopY = 3470;
		}
		break;
	case ba:
		if (position.x >= limitLeftX && position.y >= limitBottomY && fpos.y - CameraHeight / 2 >= limitBottomY)
		{
			position.x = fpos.x - CameraWidth / 2;
			position.y = fpos.y - CameraHeight / 2;
		}
		if (position.x < limitLeftX)
			position.x = limitLeftX;
		
		if (!(position.y >= limitBottomY && fpos.y - CameraHeight / 2 >= limitBottomY))
			position.y = limitBottomY;

		if (position.x > limitRightX)
		{
			stage = eCamerastage::bon;
			
		}
		break;
	case bon:
		break;
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