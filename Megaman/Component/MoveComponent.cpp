#include "MoveComponent.h"

void MoveComponent::Initialize(D3DXVECTOR2* position, D3DXVECTOR2* velocity)
{
	this->position = position;
	this->velocity = velocity;
	this->acceleration.x = 0;
	this->acceleration.y = 0;
	speed = 100;
	limitedSpeed = 200;
	bGravity = false;
	gravity = 10;
	jumpPower = 70;
	bOnAcceleration = false;
}

void MoveComponent::MoveUp()
{
	if (!bGravity)
		velocity->y = speed;
}

void MoveComponent::MoveDown()
{
	if (!bGravity)
		velocity->y = -speed;
}

void MoveComponent::Jump()
{
	if (!bGravity)
		return;
	acceleration.y = 0;
	acceleration.y = jumpPower;
	velocity->y = 0;
}

void MoveComponent::Dash(int di)
{
	if (di == 0)
		velocity->x = speed * 3;
	else
		velocity->x = -speed * 3;
}

void MoveComponent::OnDashing()
{
	bDashing = true;
}

void MoveComponent::OffDashing()
{
	bDashing = false;
}

void MoveComponent::MoveLeft()
{
	velocity->x = -speed;
	if (bDashing)
		velocity->x *=3;
}

void MoveComponent::MoveRight()
{
	velocity->x = speed;
	if (bDashing)
		velocity->x *= 3;
}

void MoveComponent::IdleX()
{
	velocity->x = 0;
}

void MoveComponent::IdleY()
{
	if (!bGravity)
		velocity->y = 0;
	else
	{
		if (abs((int)velocity->y) < limitedSpeed)
			velocity->y -= gravity;
		else
		{
			if (velocity->y < 0)
				velocity->y = limitedSpeed;
		}
	}
}

void MoveComponent::TinhVanTocDuaTrenPhuongTrinhTheoThoiGian(float ptx, float pty)
{
	//Tin Nguoi VKL!!!
	velocity->x = ptx;
	velocity->y = pty;
}

void MoveComponent::UpdateMovement(float deltatime)
{
	position->x += velocity->x * deltatime;

	position->y += velocity->y * deltatime;

	//BugCatcher(*position);
	if (bGravity && !bOnAcceleration)
	{
		if (acceleration.y > -gravity)
			acceleration.y -= gravity;
		if (acceleration.y < -gravity)
			acceleration.y = -gravity;
		velocity->y += acceleration.y;
	}

	if (bOnAcceleration)
	{
		velocity->x += acceleration.x;
		if (abs(velocity->x) > limitedSpeed)
			if (velocity->x > 0)
				velocity->x = limitedSpeed;
			else
				velocity->x = -limitedSpeed;

		velocity->y += acceleration.y;
		if (abs(velocity->y) > limitedSpeed)
			if (velocity->y > 0)
				velocity->y = limitedSpeed;
			else
				velocity->y = -limitedSpeed;
	}
}

void MoveComponent::BugCatcher(D3DXVECTOR2 position)
{
	int limitedVecAlert = 20;
	int limitedAccAlert = 80;
	if (prevPosition.x != 0 && prevPosition.y != 0)
	{
		D3DXVECTOR2 temp;
		temp.x = abs((int)position.x - (int)prevPosition.x);
		temp.y = abs((int)position.y - (int)prevPosition.y);
		if (temp.x > limitedVecAlert || temp.y > limitedVecAlert)
		{
			this;
			std::cout << "MoveComponent::BugCatcher: \t Different X: " << temp.x << "\tDifferent Y: " << temp.y << std::endl;
		}
		if (acceleration.x > limitedAccAlert || acceleration.y > limitedAccAlert)
		{
			this;
    		std::cout << "MoveComponent::BugCatcher: \t Acceleration X: " << acceleration.x << "\Acceleration Y: " << acceleration.y << std::endl;
		}
	}
	prevPosition = position;
}