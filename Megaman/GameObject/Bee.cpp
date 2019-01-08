#include "Bee.h"

std::vector<Bee*> Bee::listBee;

void Bee::MoveTo(D3DXVECTOR2 pos)
{
	float itime = 1;

	float distanceX = pos.x - this->GetPosition().x;
	float distanceY = pos.y - this->GetPosition().y;

	//if (distanceY < 0)
	//	distanceY -= 50;

	float vecX = distanceX / itime;
	float vecY = distanceY / itime;

	GetMoveComponent()->SetVelocity(vecX, vecY);
}

Bee::Bee(D3DXVECTOR2 pos, int direction)
{
	bDisable = false;
	Initialize();
	SetPosition(pos);
	if (!direction)
	{
		sprite.get()->FlipLeft();
	}
	if (direction)
		this->direction = eDirection::Left;
	else
		this->direction = eDirection::Right;
}

bool Bee::IsDisable()
{
	return bDisable;
}

void Bee::Disable()
{
	bDisable = true;
	this->box.Disable();
}

void Bee::Enable()
{
	bDisable = false;
	this->box.Enable();
}

void Bee::Initialize()
{
	Creature::Initialize();
	box.DynamicInitialize(this, 23, 22);
	box.SetPivot(12, 11);
	sprite.get()->SetAnimation("bee");
	state = eBeeState::FightADie;

	InitialzieHPComponent(12, 2);

	timeToDie = 2;
	timeToDieCount = timeToDie;
	dieTime = 0.5f;
	dieTimeCount = dieTime;

	listBee.push_back(this);
}

void Bee::ReInitialize(D3DXVECTOR2 pos, int direction)
{
	Enable();

	this->SetPosition(pos);
	box.SetPosition();
	if (!direction)
	{
		sprite.get()->FlipLeft();
	}
	if (direction)
		this->direction = eDirection::Left;
	else
		this->direction = eDirection::Right;

	state = eBeeState::FightADie;
	sprite.get()->SetAnimation("bee");
	timeToDieCount = timeToDie;
	dieTimeCount = dieTime;
}

void Bee::UpdateInput(float deltatime)
{

}

void Bee::Update(float deltatime)
{
	OnCollision(deltatime);
	GetMoveComponent()->UpdateMovement(deltatime);

	UpdateState(deltatime);

	box.SetPosition();
}

void Bee::UpdateState(float deltatime)
{

	switch (state)
	{
	case FightADie:
		break;
	case ForHonor:
		updatePos.x = Megaman::getInstance()->GetPosition().x + offset.x;
		updatePos.y = Megaman::getInstance()->GetPosition().y + offset.y;
		this->SetPosition(updatePos);	
	case NoHonor:
		timeToDieCount -= deltatime;
		if (timeToDieCount <= 0)
		{
			state = eBeeState::Dying;
			sprite.get()->SetAnimation("bee_self_destruct");
		}
		break;
	case Dying:
		dieTimeCount -= deltatime;
		if (dieTimeCount <= 0)
			Disable();
		break;
	}
}

void Bee::OnCollision(float deltatime)
{
	if (state == eBeeState::Dying)
	{
		if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox(Megaman::getInstance()->box, deltatime)))
			return;

		if (Megaman::getInstance()->state == eMegamanState::Hurt)
			return;

		Megaman::getInstance()->state = eMegamanState::Hurt;
		Megaman::getInstance()->GetHPComponent()->DoDamage(this->GetHPComponent()->GetDamage());
		return;
	}

	// Kiểm tra va chạm với đạn của Megaman
	std::vector<NormalBullet*> *listBullet = &NormalBullet::listNormalBullet;
	if (!listBullet->empty())
	{
		for (std::vector<NormalBullet*>::iterator it = listBullet->begin(); it != listBullet->end(); it++)
		{
			if ((*it)->bDisable)
				continue;
			if (!this->box.isEnable())
				continue;
			if (!Collision::IsIntersection(this->box.GetBox(), (*it)->box.GetBox()))
				continue;
			state = eBeeState::Dying;
			sprite.get()->SetAnimation("bee_self_destruct");
			this->box.Disable();
			GetMoveComponent()->IdleX();
			GetMoveComponent()->IdleY();
			(*it)->Disable();
			return;
			// Code Ở đây đoạn trừ máu của enemy
			//this->GetHPComponent()->DoDamage((*it)->damage, (*it)->bGodMode);
		}
	}

	if (state != eBeeState::FightADie)
		return;

	std::vector<MapCollision*> *listMapCollsion = &MapCollision::listMapCollision;
	if (!listMapCollsion->empty())
	{
		for (std::vector<MapCollision*>::iterator it = listMapCollsion->begin(); it != listMapCollsion->end(); it++)
		{
			if ((*it)->IsDisable())
				continue;


			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), (*it)->box.GetBox()))
				continue;

			GetMoveComponent()->IdleX();
			GetMoveComponent()->IdleY();
			state = eBeeState::NoHonor;
			cout << this->box.GetBox().position.y << endl;
		}
	}

	for (std::vector<Bee*>::iterator it = listBee.begin(); it != listBee.end(); it++)
	{
		if ((*it)->state == eBeeState::ForHonor)
			return;
	}

	if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox(Megaman::getInstance()->box, deltatime)))
		return;
	
	GetMoveComponent()->IdleX();
	GetMoveComponent()->IdleY();

	offset.x = this->GetPosition().x - Megaman::getInstance()->GetPosition().x;
	offset.y = this->GetPosition().y - Megaman::getInstance()->GetPosition().y;

	state = eBeeState::ForHonor;
}

void Bee::Draw()
{
	Creature::Draw();
}

void Bee::Destroy()
{
}

void Bee::UpdateAll(float deltatime)
{
	if (PRINT_SIZE && !PRINT_SIZE_ENABLE_OBJECT)
	{
		static float printTimeCount = 0;
		if (printTimeCount >= 1.0)
		{
			cout << "Bee's Object Amount: " << listBee.size() << endl;
			printTimeCount = 0;
		}
		printTimeCount += deltatime;
	}

	for (std::vector<Bee*>::iterator it = listBee.begin(); it != listBee.end(); it++)
	{
		if (!(*it)->IsDisable())
			(*it)->Update(deltatime);
	}

	if (PRINT_SIZE_ENABLE_OBJECT)
	{
		static float printTimeCount2 = 0;
		if (printTimeCount2 >= 1.0)
		{
			int count = 0;
			for (std::vector<Bee*>::iterator it = listBee.begin(); it != listBee.end(); it++)
			{
				if (!(*it)->IsDisable())
					count++;
			}

			cout << "Bee's Object Enable Amount: " << count << "/" << listBee.size() << endl;
			printTimeCount2 = 0;
		}
		printTimeCount2 += deltatime;
	}
}

void Bee::DrawAll()
{
	for (std::vector<Bee*>::iterator it = listBee.begin(); it != listBee.end(); it++)
	{
		if (!(*it)->IsDisable())
			(*it)->Draw();
	}
}

Bee * Bee::CreateBee(D3DXVECTOR2 pos, int direction)
{
	for (int i = 0; i < listBee.size(); i++)
	{
		if (listBee[i]->IsDisable())
		{
			listBee[i]->ReInitialize(pos, direction);
			return listBee[i];
		}
	}
	Bee* head = new Bee(pos, direction);
	return head;
}
