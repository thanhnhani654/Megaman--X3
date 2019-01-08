#pragma once
#include "GameObject.h"
#include "../Component/HPComponent.h"
#include "../Component/MoveComponent.h"
#include "../Component/Box2D.h"


class Creature : public GameObject
{
private:
	MoveComponent moveComponent;
	HPComponent hpComponent;			// Thêm cho nhiều chơi
	
public:
	Box2D box;
	#pragma region Inherit

	virtual void Initialize();
	virtual void Ghost_Initialize();				//Initialize mà không lưu lại trong listObject
	virtual void UpdateInput(float deltatime);
	virtual void Update(float deltatime);
	virtual void Draw();
	virtual void OnCollision(GameObject* object, float collideTime, int normalX, int normalY) {}
	virtual void Destroy();							//Dùng để xóa những con trỏ bên trong

	#pragma endregion

	#pragma region GetSet

	MoveComponent* GetMoveComponent()
	{
		return &moveComponent;
	}

	void InitialzieHPComponent(float hp, float dmg) { hpComponent.Initialize(hp, dmg); }
	
	HPComponent* GetHPComponent()
	{
		return &hpComponent;
	}

	#pragma endregion
};