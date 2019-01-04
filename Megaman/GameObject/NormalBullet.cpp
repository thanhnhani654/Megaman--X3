#include "NormalBullet.h"

std::vector<NormalBullet*> NormalBullet::listNormalBullet;

void NormalBullet::Ghost_Initialize(float x, float y, eDirection idirection, int lv, float dmg, bool godMode)
{
	GameObject::Ghost_Initialize();
	lifeTime = 5.0f;
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	speed = 5;
	damage = dmg;
	bGodMode = godMode;

	box.DynamicInitialize(this, 8, 6);
	switch (lv)
	{
	case 1:
		sprite.get()->SetAnimation("char_bullet");
		break;
	case 2:
		sprite.get()->SetAnimation("char_bullet_lv2");
		break;
	case 3:
		sprite.get()->SetAnimation("char_bullet_lv3");
		break;
	}

	if (idirection == eDirection::Left)
		sprite.get()->FlipLeft();
	
	GetTagMethod()->AddTag(eTag::PlayerBulletTag);

	listNormalBullet.push_back(this);
}

void NormalBullet::Re_Initialize(float x, float y, eDirection idirection, int lv, float dmg, bool godMode)
{
	lifeTime = 5.0f;
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	speed = 5;
	damage = dmg;
	bGodMode = godMode;

	switch (lv)
	{
	case 1:
		sprite.get()->SetAnimation("char_bullet");
		break;
	case 2:
		sprite.get()->SetAnimation("char_bullet_lv2");
		break;
	case 3:
		sprite.get()->SetAnimation("char_bullet_lv3");
		break;
	}

	if (idirection == eDirection::Left)
		sprite.get()->FlipLeft();
}

void NormalBullet::OnCollision(float deltatime)
{
	std::vector<NotorBanger*> *listNotorBanger = &NotorBanger::listNotorBanger;
	if (!listNotorBanger->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<NotorBanger*>::iterator it = listNotorBanger->begin(); it != listNotorBanger->end(); it++)
		{
			if ((*it)->IsDisable())
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			//cout << "A" << endl;
			//(*it)->GetHPComponent()->DoDamage(damage, bGodMode);
			//Disable();
		}
	}
	///////////////Check With HeadGunner////////////////////////////
	std::vector<HeadHunter*> *listHeadHunter = &HeadHunter::listHeadHunter;
	if (!listHeadHunter->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<HeadHunter*>::iterator it = listHeadHunter->begin(); it != listHeadHunter->end(); it++)
		{
			if ((*it)->IsDisable())
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			(*it)->GetHPComponent()->DoDamage(damage, bGodMode);
			Disable();
		}
	}
	///////////////Check With Helit////////////////////////////
	std::vector<Helit*> *listHelit = &Helit::listHelit;
	if (!listHelit->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<Helit*>::iterator it = listHelit->begin(); it != listHelit->end(); it++)
		{
			if ((*it)->IsDisable())
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			(*it)->GetHPComponent()->DoDamage(damage, bGodMode);
			Disable();
		}
	}
	///////////////Check With NotorBullet////////////////////////////
	std::vector<NotorBullet*> *listNotorBullet = &NotorBullet::listNotorBullet;
	if (!listNotorBullet->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<NotorBullet*>::iterator it = listNotorBullet->begin(); it != listNotorBullet->end(); it++)
		{
			if ((*it)->bDisable)
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			(*it)->GetHPComponent()->DoDamage(damage, bGodMode);
			Disable();
		}
	}
	///////////////Check With NotorBullet////////////////////////////
	std::vector<Rocket*> *listRocket = &Rocket::listRocket;
	if (!listRocket->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<Rocket*>::iterator it = listRocket->begin(); it != listRocket->end(); it++)
		{
			if ((*it)->bDisable)
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			(*it)->GetHPComponent()->DoDamage(damage, bGodMode);
			Disable();
		}
	}
}

void NormalBullet::CountDown(float deltatime)
{
	lifeTime -= deltatime;
}

void NormalBullet::Disable()
{
	bDisable = true;
}

void NormalBullet::Update(float deltatime)
{
	if (bDisable)
		return;
	if (direction == eDirection::Left)
		GetPosition_Ptr()->x -= speed;
	else
		GetPosition_Ptr()->x += speed;
	box.SetPosition();
	//OnCollision(deltatime);

	CountDown(deltatime);
	if (lifeTime <= 0)
		Disable();
}

void NormalBullet::Draw()
{
	if (!bDisable)
	GameObject::Draw();
}
