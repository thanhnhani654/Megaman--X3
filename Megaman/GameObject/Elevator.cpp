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
	bWaiting = true;
	bStop = false;
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

	bWaiting = true;
	bStop = false;
	stopPoint = 3633;

	listElevator.push_back(this);
}

void Elevator::Reset()
{
	this->SetPosition(this->GetPosition().x, 3130);
	bWaiting = true;
	bStop = false;
}

void Elevator::UpdateInput(float deltatime)
{
	GetMoveComponent()->MoveUp();

	box.SetPosition();
}

void Elevator::Update(float deltatime)
{
	if (bWaiting)
		return;
	if (bStop)
		return;
	if (this->GetPosition().y >= stopPoint)
		bStop = true;

	this->GetMoveComponent()->UpdateMovement(deltatime);
}

void Elevator::Draw()
{
	Creature::Draw();
}