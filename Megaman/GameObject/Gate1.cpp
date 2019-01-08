#include "Gate1.h"

std::vector<Gate1*> Gate1::listGate1;

void Gate1::Initialize()
{
	box.StaticInitialize(this, 20, 20);
	listGate1.push_back(this);
	name = "null";
	GetTagMethod()->AddTag(eTag::WallTag);
	sprite.get()->SetAnimation("gate_1_open");
}

void Gate1::Initialize(float x, float y, int w, int h)
{
	Creature::Initialize();
	SetPosition(x, y);
	box.DynamicInitialize(this, 16, 48);
	box.SetPivot(8, 24);

	name = "gate";
	GetTagMethod()->AddTag(eTag::WallTag);
	sprite.get()->SetAnimation("gate_1", false);
	sprite.get()->SetFrameRate(4);

	listGate1.push_back(this);
}

void Gate1::GateOpen()
{
	sprite.get()->SetAnimation("gate_1_open", false);
}

void Gate1::GateClosed()
{
	sprite.get()->SetAnimation("gate_1_close", false);
}

void Gate1::Update(float deltatime)
{
	box.SetPosition();
}

void Gate1::Draw()
{
	Creature::Draw();
}