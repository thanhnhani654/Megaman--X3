#pragma once
#include "Creature.h"
#include "../Component/Box2D.h"
#include "../Component/Collision.h"
#include "MapCollision.h"
#include <vector>

class HelitRocket : public Creature
{
private:
	float lifeTime;
	float lifeTimeCount;
	eDirection direction;
	float speed;


public:
	bool bDisable;

	static std::vector<HelitRocket*> listHelitRocket;

	void Ghost_Initialize(float x, float y, eDirection idirection);				//Initialize mà không lưu lại trong listObject
	void Re_Initialize(float x, float y, eDirection idirection);

	void OnCollision(float deltatime);

	void Disable();
	void Update(float deltatime);
	void Draw();
	static void UpdateAll(float deltaime);
	static void DrawAll();
};