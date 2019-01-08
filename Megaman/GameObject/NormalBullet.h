#pragma once
#include "GameObject.h"
#include "../Component/Box2D.h"
#include <vector>
#include "../Component/Collision.h"
//#include "Bee.h"
#include "BlastHornet.h"
#include "HeadHunter.h"
#include "Helit.h"
#include "NotorBanger.h"
#include "Bee.h"

class NormalBullet : public GameObject
{
private:
	float lifeTime;
	float lifeTimeCount;
	eDirection direction;
	float speed;
	
	
public:
	Box2D box;
	bool bDisable;
	float damage;
	bool bGodMode;

	static std::vector<NormalBullet*> listNormalBullet;

	void Ghost_Initialize(float x, float y, eDirection idirection, int lv, float dmg, bool godMode =false);				//Initialize mà không lưu lại trong listObject
	void Re_Initialize(float x, float y, eDirection idirection, int lv, float dmg, bool godMode = false);
	void OnCollision(float deltatime);

	void CountDown(float deltatime);
	void Disable();
	void Update(float deltatime);
	void Draw();
};