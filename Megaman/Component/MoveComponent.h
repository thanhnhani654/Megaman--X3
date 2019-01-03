#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>
#include <iostream>

class MoveComponent
{
private:
	D3DXVECTOR2* position;
	D3DXVECTOR2  prevPosition;
	D3DXVECTOR2* velocity;
	D3DXVECTOR2 acceleration;
	float speed;
	bool bGravity;					//Bằng true cho Player. Tất cả mấy cái còn lại không cần
	float gravity;					//Khi bGravity bằng true thì acceleration.y = gravity
	float limitedSpeed;				//Nên là số nguyên
	float jumpPower;
	bool bOnAcceleration;
	bool bDashing;
public:

	#pragma region GetSet
	D3DXVECTOR2 GetPosition()
	{
		return *position;
	}

	D3DXVECTOR2 GetVelocity()
	{
		return *velocity;
	}

	D3DXVECTOR2 GetAcceleration()
	{
		return acceleration;
	}

	void SetAcceleration(D3DXVECTOR2 acc)
	{
		acceleration = acc;
	}

	void SetAcceleration(float x, float y)
	{
		acceleration.x = x;
		acceleration.y = y;
	}

	void SetVelocity(D3DXVECTOR2 vec)
	{
		acceleration = vec;
	}

	void SetVelocity(float x, float y)
	{
		velocity->x = x;
		velocity->y = y;
	}


	float GetSpeed()
	{
		return speed;
	}

	void SetSpeed(float spd)
	{
		speed = spd;
	}

	void EnableGravity()
	{
		bGravity = true;
	}

	void DisableGravity()
	{
		bGravity = false;
	}

	float GetGravity()
	{
		return gravity;
	}

	void SetGravity(float gra)
	{
		gravity = gra;
	}

	float GetLimitedSpeed()
	{
		return limitedSpeed;
	}

	void SetLimitedSpeed(float limit)
	{
		limitedSpeed = limit;
	}

	float GetJumpPower()
	{
		return jumpPower;
	}

	void SetJumpPower(float jp)
	{
		jumpPower = jp;
	}

	void OnAcceleration()
	{
		bOnAcceleration = true;
	}

#pragma endregion

	void Initialize(D3DXVECTOR2 *position, D3DXVECTOR2 *velocity);
	void MoveUp();					//Chỉ dùng khi bGravity False
	void MoveDown();				//Chỉ dùng khi bGravity False
	void Jump();					//Chỉ dùng khi bGravity True
	void Dash(int);
	void OnDashing();
	void OffDashing();
	void MoveLeft();
	void MoveRight();
	void IdleX();
	void IdleY();
	void TinhVanTocDuaTrenPhuongTrinhTheoThoiGian(float ptx, float pty);			//Điều kiện sử dụng là bGravity = false và không sử dụng bất kỳ hàm nào khác
	


	void UpdateMovement(float deltatime);
	void BugCatcher(D3DXVECTOR2 position);
};