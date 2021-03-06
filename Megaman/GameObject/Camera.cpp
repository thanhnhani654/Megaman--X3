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

bool Camera::IsFollowCamera()
{
	if (!bFollowCamera && bFreeMode)
		return true;
	return false;
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
		std::cout << "X: " << position.x << " Y: " << position.y << std::endl;
	}
}

void Camera::UpdateCameraStage(float deltatime, D3DXVECTOR2 fpos)
{
	switch (stage)
	{
	case khong:
		position.x = 200;
		position.y = 3020;
		limitLeftX = 0;
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
			limitBottomY = 3020;
			limitTopY = 3550;
		}
		break;
	case hai:
		if (position.y >= limitBottomY && fpos.y - CameraHeight / 2 >= limitBottomY)
			position.y = fpos.y - CameraHeight / 2;
		else
			position.y = limitBottomY;
		
		if (position.y > limitTopY)
		{
			stage = eCamerastage::haichamnam;
			//limitLeftX = 970;
			limitLeftX = 775;
			//limitRightX = 2070;
			limitRightX = 1300;
			limitBottomY = 2745;
			limitTopY = 3470;
		}
		break;
	case haichamnam:
		if (position.x >= limitLeftX)
			position.x = fpos.x - CameraWidth / 2;
		else
			position.x = limitLeftX;

		if (position.x > limitRightX)
		{
			stage = eCamerastage::ba;
			limitLeftX = 1300;
			limitRightX = 2070;
			limitBottomY = 2745;
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
			limitLeftX = 2000;
			limitRightX = 2200;
		}
		break;
	case bon:
		//position.x = 2312;
		
		if (position.x >= limitLeftX)
		{
			position.x = fpos.x - CameraWidth / 2;
		}
		else
			position.x = limitLeftX;
		
		if (position.x > limitRightX)
		{
			stage = eCamerastage::nam;
			limitLeftX = 2516;
			limitRightX = 5400;
			stage = eCamerastage::HornetFight;
		}
		break;
	case HornetFight:
		position.x = 2310;
		limitLeftX = 2310;
		limitRightX = 5400;
		break;
	case nam:
		position.x = 2310;
		if (position.x >= limitLeftX && position.y >= limitBottomY && fpos.y - CameraHeight / 2 >= limitBottomY)
		{
			position.x = fpos.x - CameraWidth / 2;
			position.y = fpos.y - CameraHeight / 2;
		}
		if (position.x < limitLeftX)
			position.x = limitLeftX;
		
		if (position.x > limitRightX)
		{
			stage = eCamerastage::sau;
			limitLeftX = 5650;
			limitRightX = 5650;
		}
			break;
	
	case sau:
		position.x = 5650;

		if (position.x >= limitLeftX)
		{
			position.x = fpos.x - CameraWidth / 2;
		}

		if (position.x > limitRightX)
		{
			stage = eCamerastage::bay;
			limitBottomY = 2000;
			limitRightX = 6150;
		}
		break;
	case bay:
		position.x = 5900;
		
		if (position.y >= limitBottomY && fpos.y - CameraHeight / 2 >= limitBottomY)
		{
			position.y = fpos.y - CameraHeight / 2;
		}

		if (position.x >= limitLeftX)
		{
			position.x = fpos.x - CameraWidth / 2;
		}
		
		if (position.x > limitRightX)
		{
			stage = eCamerastage::tam;
			limitLeftX = 6150;
			limitRightX = 7165;
		}
		break;
	case tam:
		position.x = 6150;

		if (position.x >= limitLeftX)
		{
			position.x = fpos.x - CameraWidth / 2;
		}

		if (position.x > limitRightX)
		{
			stage = eCamerastage::chin;
			limitLeftX = 7165;
			limitRightX = 7165;
		}

		break;
	case chin:
		position.x = 7165;

		if (position.x >= limitLeftX)
		{
			position.x = fpos.x - CameraWidth / 2;
		}

		/*if (position.x > limitRightX)
		{
			stage = eCamerastage::chin;
			limitLeftX = 7165;
			limitRightX = 7165;
		}*/

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