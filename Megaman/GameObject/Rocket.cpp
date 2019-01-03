#include "Rocket.h"

std::vector<Rocket*> Rocket::listRocket;

void Rocket::Ghost_Initialize(float x, float y, eDirection idirection)
{
	Creature::Ghost_Initialize();
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	box.DynamicInitialize(this, 8, 6);
	sprite.get()->SetAnimation("notorbanger_bullet");

	GetMoveComponent()->DisableGravity();
	GetMoveComponent()->SetSpeed(150);

	GetTagMethod()->AddTag(eTag::RocketTag);

	listRocket.push_back(this);
}

void Rocket::Re_Initialize(float x, float y, eDirection idirection)
{
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	sprite.get()->SetAnimation("headgunner_rocket");
	GetMoveComponent()->Jump();
}

void Rocket::OnCollision(float deltatime)
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

void Rocket::Disable()
{
	bDisable = true;
}

void Rocket::Update(float deltatime)
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

void Rocket::Draw()
{
	Creature::Draw();
}

void Rocket::UpdateAll(float deltaime)
{
	for (int i = 0; i < listRocket.size(); i++)
	{
		if (listRocket[i]->bDisable)
			continue;
		listRocket[i]->Update(deltaime);
	}
}

void Rocket::DrawAll()
{
	for (int i = 0; i < listRocket.size(); i++)
	{
		if (listRocket[i]->bDisable)
			continue;
		listRocket[i]->Draw();
	}
}
