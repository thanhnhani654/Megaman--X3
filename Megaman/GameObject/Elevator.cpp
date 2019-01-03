#include "Elevator.h"

std::vector<Elevator*> Elevator::listElevator;

void Elevator::Initialize()
{
	box.StaticInitialize(this, 20, 20);
	listElevator.push_back(this);
	name = "null";
	GetTagMethod()->AddTag(eTag::GroundTag);
	GetMoveComponent()->SetSpeed(150);
	GetMoveComponent()->DisableGravity();
	sprite.get()->SetAnimation("lift_1");
}

void Elevator::Initialize(float x, float y, int w, int h)
{
	Creature::Initialize();
	SetPosition(x, y);
	box.DynamicInitialize(this, 96, 24);
	box.SetPivot(48, 12);

	name = "ground";
	GetTagMethod()->AddTag(eTag::GroundTag);
	GetMoveComponent()->SetSpeed(40);
	GetMoveComponent()->DisableGravity();
	sprite.get()->SetAnimation("lift_1");

	listElevator.push_back(this);
}

void Elevator::UpdateInput(float deltatime)
{
	GetMoveComponent()->MoveUp();

	box.SetPosition();
}

void Elevator::Update(float deltatime)
{
	this->GetMoveComponent()->UpdateMovement(deltatime);
}

void Elevator::Draw()
{
	Creature::Draw();
}