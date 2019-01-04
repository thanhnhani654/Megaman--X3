#pragma once
#include "Creature.h"
#include "Megaman.h"
#include "NormalBullet.h"
#include "NotorBullet.h"
#include "Rocket.h"

#define PRINT_SIZE FALSE
#define PRINT_SIZE_ENABLE_OBJECT FALSE

enum eHeadHunterState
{
	FireRocket,
	FireBullet,
	onHHDead
};

class HeadHunter : public Creature
{
private:
	eDirection direction;
	eHeadHunterState state;

	//Method
	//void GetDirection();

	float changeStateTime;
	float changeStateTimeCount;

	float waitForFireTime;
	float waitForFireTimeCount;
	float fireInteval;
	float fireIntevalCount;
	float fireTimesCount;
	float timeToDead;
	float timeToDeadCount;

	bool bDisable;


public:
	static std::vector<HeadHunter*> listHeadHunter;
	HeadHunter() {};
	HeadHunter(D3DXVECTOR2 pos, int direction = 0);
	~HeadHunter() {};

	void FireBullet();
	void FireRocket();
	void FireBulletUpdate(float deltatime);
	void FireRocketUpdate(float deltatime);
	void ResetFireBulletTime();
	void ResetRocketTime();
	bool IsDisable();
	void Disable();
	void Enable();

	void Initialize();
	void ReInitialize(D3DXVECTOR2 pos, int direction = 0);
	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void UpdateState(float deltatime);
	void OnCollision(float deltatime);
	void Draw();
	void Destroy();							//Dùng để xóa những con trỏ bên trong


	static void UpdateAll(float deltatime);
	static void DrawAll();
	static HeadHunter* CreateHeadHunter(D3DXVECTOR2 pos, int direction = 0);
};