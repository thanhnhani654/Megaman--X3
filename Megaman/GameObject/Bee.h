#pragma once
#include "Creature.h"
#include "Megaman.h"
//#include "NormalBullet.h"
#include "NotorBullet.h"
#include "Rocket.h"

#define PRINT_SIZE FALSE
#define PRINT_SIZE_ENABLE_OBJECT FALSE

enum eBeeState
{
	FightADie,
	ForHonor,
	NoHonor,
	Dying,
};

class Bee : public Creature
{
private:
	eDirection direction;

	eBeeState state;

	bool bDisable;

	//Megaman* megaman;

	D3DXVECTOR2 offset;
	D3DXVECTOR2 updatePos;

	float timeToDie;
	float timeToDieCount;
	float dieTime;
	float dieTimeCount;
	float timeToDead;
	float timeToDeadCount;


public:
	static std::vector<Bee*> listBee;
	Bee() {};
	Bee(D3DXVECTOR2 pos, int direction = 0);
	~Bee() {};

	void MoveTo(D3DXVECTOR2 pos);
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
	static Bee* CreateBee(D3DXVECTOR2 pos, int direction = 0);
};