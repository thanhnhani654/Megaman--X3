#pragma once
#include "Creature.h"
#include "../Controllable.h"
#include "../Component/Collision.h"
#include "Item.h"
#include "MapCollision.h"
#include "NormalBullet.h"
#include "Elevator.h"
#include "Gate1.h"
#include "NotorBanger.h"
#include "HeadHunter.h"
#include "Helit.h"
#include "NotorBullet.h"
#include "Rocket.h"

#define DEBUG_IMMORTAL TRUE			//TRUE: không va chạm với Enemy hay bullet | FALSE: Va chạm với Enemy và bullet

enum class eMegamanState			
{
	Stand,
	StandShoot,
	Run,
	RunShoot,
	Jump,
	JumpShoot,
	Dash,
	ClingWall,
	Hurt,
};

class Megaman : public Creature, Controllable
{
private:
	
	static Megaman* instance;
	eMegamanState prevState;
	eMegamanState state;
	eDirection direction;
	float chargeLv2;
	float chargeLv3;
	float chargeBar;

	//State Conditioner
	bool bShoot;
	bool bMoving;
	bool bJump;
	bool bDash;
	bool bClingWall;
	bool bEndHurt;
	bool bImmortal;
	bool bPassingGate;
	bool bDisableInput;

	//Attribute
	float limitDashTime;
	float dashTime;
	float shootCountTime;
	float shootTime;
	float hurtTime;
	float hurtTimeCount;
	float immortalTime;
	float immortalCountTime;
	float waitOpenGateTime;
	float waitOpenGateTimeCount;
	float timePassGate;
	float timePassGateCount;
	int hurtEffect;
	int hurtEffectCount;


	Gate1* tempGate;
	
	
	// Method
	void MovementController(float deltatime);
	void GroundCollision(float collideTime, int normalX, int normalY, float deltatime, Box box);
	//void LeftUpGroundCollision(float collideTime, int normalX, int normalY, float deltatime, Box box);		//Lam cai ground mai nha ma chua dc
	void WallCollision(float collideTime, int normalX, int normalY, float deltatime, Box box);
	void ElevatorCollision(float collideTime, int normalX, int normalY, float deltatime, Box box, float eSpeed);
	void GateCollision(float collideTime, int normalX, int normalY, float deltatime, Box box, float eSpeed);
	void EnemyCollision(int direction);				//-1: Left, 1: Right
	void FireBullet();
	void ChargeUp(float deltatime);
	void Jump();
	void ShootCountDown(float deltatime);
	void HurtCountDown(float deltatime);
	void PassGate(float deltatime);
	void CameraController(float deltatime);
	

public:
	#pragma region Inherit

	//Inherit From Creature
	void Initialize();
	void Ghost_Initialize();				//Initialize mà không lưu lại trong listObject
	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void UpdateBullet(float deltatime);
	void UpdateState(float deltatime);
	void UpdateAnimation();
	void Draw();
	//Chua dung toi hoac co the khong dung toi luon
	void OnCollision(GameObject* object, float collideTime, int normalX, int normalY);
	void OnCollision(float deltatime);
	void Destroy();							//Dùng để xóa những con trỏ bên trong

	//Inherit From Controllable
	void OnKeyDown(int Keycode);
	void OnKeyUp(int Keycode);

	#pragma endregion

	
	static Megaman* getInstance();
};