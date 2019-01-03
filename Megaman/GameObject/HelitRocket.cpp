#include "HelitRocket.h"

std::vector<HelitRocket*> HelitRocket::listHelitRocket;

void HelitRocket::Ghost_Initialize(float x, float y, eDirection idirection)
{
	Creature::Ghost_Initialize();
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	box.DynamicInitialize(this, 8, 6);
	sprite.get()->SetAnimation("helit_rocket");

	GetMoveComponent()->DisableGravity();
	GetMoveComponent()->SetSpeed(150);

	GetTagMethod()->AddTag(eTag::HelitRocketTag);

	listHelitRocket.push_back(this);
}

void HelitRocket::Re_Initialize(float x, float y, eDirection idirection)
{
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	sprite.get()->SetAnimation("headgunner_rocket");
	GetMoveComponent()->Jump();
}

void HelitRocket::OnCollision(float deltatime)
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

void HelitRocket::Disable()
{
	bDisable = true;
}

void HelitRocket::Update(float deltatime)
{
	if (bDisable)
		return;
	if (direction == eDirection::Left)
		GetMoveComponent()->MoveLeft();
	else
		GetMoveComponent()->MoveRight();
	GetMoveComponent()->UpdateMovement(deltatime);


	box.SetPosition();

	OnCollision(deltatime);
}

void HelitRocket::Draw()
{
	Creature::Draw();
}

void HelitRocket::UpdateAll(float deltaime)
{
	for (int i = 0; i < listHelitRocket.size(); i++)
	{
		if (listHelitRocket[i]->bDisable)
			continue;
		listHelitRocket[i]->Update(deltaime);
	}
}

void HelitRocket::DrawAll()
{
	for (int i = 0; i < listHelitRocket.size(); i++)
	{
		if (listHelitRocket[i]->bDisable)
			continue;
		listHelitRocket[i]->Draw();
	}
}
