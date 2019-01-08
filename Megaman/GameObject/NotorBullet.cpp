#include "NotorBullet.h"

std::vector<NotorBullet*> NotorBullet::listNotorBullet;

void NotorBullet::Ghost_Initialize(float x, float y, eDirection idirection)
{
	Creature::Ghost_Initialize();
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	box.DynamicInitialize(this, 8, 6);
	box.SetPivot(4, 3);
	sprite.get()->SetAnimation("notorbanger_bullet");

	InitialzieHPComponent(1, 1);

	GetMoveComponent()->EnableGravity();
	GetMoveComponent()->SetSpeed(150);
	GetMoveComponent()->SetJumpPower(100);
	GetMoveComponent()->Jump();

	GetTagMethod()->AddTag(eTag::NotorBulletTag);
	lifeTime = 3;
	//lifeTimeCount = 3;

	listNotorBullet.push_back(this);
}

void NotorBullet::Re_Initialize(float x, float y, eDirection idirection)
{
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	sprite.get()->SetAnimation("notorbanger_bullet");
	lifeTime = 3;
	GetMoveComponent()->Jump();
}

void NotorBullet::OnCollision(float deltatime)
{
	std::vector<MapCollision*> *listMapCollsion = &MapCollision::listMapCollision;
	if (!listMapCollsion->empty())
	{
		for (std::vector<MapCollision*>::iterator it = listMapCollsion->begin(); it != listMapCollsion->end(); it++)
		{

			if (!Collision::IsIntersection(this->box.GetBox(), (*it)->box.GetBox()))
				continue;
			
			this->Disable();
		}
	}
}

void NotorBullet::Disable()
{
	bDisable = true;
}

void NotorBullet::Update(float deltatime)
{
	if (bDisable)
		return;
	if (direction == eDirection::Left)
		GetMoveComponent()->MoveLeft();
	else
		GetMoveComponent()->MoveRight();
	GetMoveComponent()->UpdateMovement(deltatime);
	
	lifeTime -= deltatime;
	if (lifeTime < 0)
		Disable();

	box.SetPosition();

	OnCollision(deltatime);
}

void NotorBullet::Draw()
{
	Creature::Draw();
}

void NotorBullet::UpdateAll(float deltaime)
{
	for (int i = 0; i < listNotorBullet.size(); i++)
	{
		if (listNotorBullet[i]->bDisable)
			continue;
		listNotorBullet[i]->Update(deltaime);
	}
}

void NotorBullet::DrawAll()
{
	for (int i = 0; i < listNotorBullet.size(); i++)
	{
		if (listNotorBullet[i]->bDisable)
			continue;
		listNotorBullet[i]->Draw();
	}
}
