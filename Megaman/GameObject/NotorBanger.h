#pragma once
#include "Creature.h"
#include "Megaman.h"
#include "NormalBullet.h"
#include "NotorBullet.h"
#include "Item.h"

#define PRINT_SIZE FALSE
#define PRINT_SIZE_ENABLE_OBJECT FALSE


//Ghép 2 trạng thái Fire với Posture lại
enum eNotorBangerState
{
	Jump,
	Fire1,
	Fire2,
	onNBDead
};

class NotorBanger : public Creature
{
private:
	eNotorBangerState state;
	eDirection direction;
	float fire1Time;
	float fire1TimeCount;

	//Method
	void GetDirection();
	void WallCollision(float collideTime, int normalX, int normalY, float deltatime, Box box);

	float waitForFireTime;
	float waitForFireTimeCount;
	float fireInteval;
	float fireIntevalCount;
	float fireTimesCount;
	float timeToDead;
	float timeToDeadCount;

	
	float demTime;

public:
	static std::vector<NotorBanger*> listNotorBanger;
	NotorBanger() {};
	NotorBanger(D3DXVECTOR2 pos, int direct = 0);
	~NotorBanger() {};

	bool bDisable;
	bool bJumping;
	bool bFire1;
	bool bFire2;
	bool bFireStyle;

	void FireBullet();
	void FireBulletUpdate(float deltatime);
	void ResetFireTime();
	bool IsDisable();
	void Disable();
	void Disable(int o);
	void Enable();

	void Initialize();
	void ReInitialize(D3DXVECTOR2 pos, int direct = 0);
	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void UpdateState(float deltatime);
	void OnCollision(float deltatime);
	void Draw();
	void Destroy();							//Dùng để xóa những con trỏ bên trong


	static void UpdateAll(float deltatime);
	static void DrawAll();
	static NotorBanger* CreateNotorBanger(D3DXVECTOR2 pos, int direct = 0);
};