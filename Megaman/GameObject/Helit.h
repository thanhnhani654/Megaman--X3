#pragma once
#include "Creature.h"
#include "Megaman.h"
#include "NormalBullet.h"
#include "HelitRocket.h"

#define PRINT_SIZE FALSE
#define PRINT_SIZE_ENABLE_OBJECT FALSE

enum eHelitState
{
	GoDown,
	GoUp,
	Mad,
	Fire,
	onHDead
};


class Helit : public Creature
{
private:
	Sprite subSprite;
	eHelitState state;
	eDirection direction;

	float fireTimeInterval;
	float fireTimeIntervalCount;
	float fireCount;

	float madTime1;
	float madTime1Count;
	float madTime2;
	float madTime2Count;
	int   madstage;
	float tempY;
	float timeToDead;
	float timeToDeadCount;

	bool bMad;
	bool bDisable;
	

public:
	static std::vector<Helit*> listHelit;
	Helit() {};
	Helit(D3DXVECTOR2 pos, int  direction = 0);
	~Helit() {};


	void GoDownFindPlayer(float deltatime);
	void RunAway();
	void MadUpdate(float deltatime);
	void ResetMadTime();

	void FireRocket();
	bool IsDisable();
	void Disable();
	void Disable(int o);
	void Enable();

	void Initialize();
	void ReInitialize(D3DXVECTOR2 pos, int  direction = 0);
	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void UpdateState(float deltatime);
	void OnCollision(float deltatime);
	void Draw();
	void Destroy();							//Dùng để xóa những con trỏ bên trong


	static void UpdateAll(float deltatime);
	static void DrawAll();
	static Helit* CreateHelit(D3DXVECTOR2 pos, int  direction = 0);
};