#pragma once
#include "Megaman.h"
#include "NormalBullet.h"
#include "HUD.h"
#include "Bee.h"
#include <math.h>
#include <random>

#define PRINT_SIZE FALSE
#define PRINT_SIZE_ENABLE_OBJECT FALSE

enum eBlastHornetState
{
	DetectTarget,				//Nhắm hướng đâm kim
	DirectAttack,				//Đâm Kim
	Back,
	AngleAttack,				//Thả thiên thần
	HuhuKheckhec,				//Lượn một chút
	onBHDead
};

enum eMysteryState
{
	giaidoan1,
	giaidoan2,
	giaidoan3,
	giaidoan4
};

class BlastHornet : public Creature
{
private:
	eDirection direction;
	eBlastHornetState state;
	Sprite subSprite;
	
	//Specific Attribute
	float detectTargetTime;
	float detectTargetTimeCount;
	D3DXVECTOR2 targetLocation;
	D3DXVECTOR2 attackPosition1;			//Nho vi tri truoc khi tan cong de ve
	D3DXVECTOR2 attackPosition2;
	D3DXVECTOR2 point1;
	D3DXVECTOR2 point2;
	float r;
	D3DXVECTOR2 giaoDiem;
	eMysteryState moveState;
	int nextMove;						//Xác định vị trí dừng tiếp theo của chuỗi hành động là point 1 hay point 2
	int prevMove;
	float timeLine;
	float attackTime;					//Tong thoi gian can cho mot lan tan cong
	float attackTimeCount;
	float backTime;
	float backTimeCount;
	float angleAttackTime;
	float angleAttackTimeCount;
	float changeStateTime;
	float changeStateTimeCount;
	float timeToDead;
	float timeToDeadCount;

	bool bDisable;

	//Specific Method
	void MoveTo(D3DXVECTOR2 itargetLocation, float itime);
	void MysteryMove(D3DXVECTOR2 itargetLocation);
	void LayTam();
	void PhuongTrinhChuyenDongTron(float t);
	void RandomNextMove();
	void Fire();
	int GetDirection();							//1: Megaman nằm ở bên trái, 0: Megaman nằm ở bên phải
	void FaceTarget();							//Luôn nhìn về hướng mục tiêu


public:
	static std::vector<BlastHornet*> listBlastHornet;
	BlastHornet() {};
	BlastHornet(D3DXVECTOR2 pos, int direction = 0);
	~BlastHornet() {};

	bool bStartBossFight;

	bool IsDisable();
	void Disable();
	void Disable(int i);
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
	static BlastHornet* CreateBlastHornet(D3DXVECTOR2 pos, int direction = 0);
};