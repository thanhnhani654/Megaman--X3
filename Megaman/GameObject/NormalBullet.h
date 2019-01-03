#pragma once
#include "GameObject.h"
#include "../Component/Box2D.h"
#include <vector>

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

	static std::vector<NormalBullet*> listNormalBullet;

	void Ghost_Initialize(float x, float y, eDirection idirection, int lv);				//Initialize mà không lưu lại trong listObject
	void Re_Initialize(float x, float y, eDirection idirection, int lv);

	void CountDown(float deltatime);
	void Disable();
	void Update(float deltatime);
	void Draw();
};