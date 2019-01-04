#include "HeadHunter.h"

std::vector<HeadHunter*> HeadHunter::listHeadHunter;

HeadHunter::HeadHunter(D3DXVECTOR2 pos, int direction)
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

void HeadHunter::FireBullet()
{
	std::vector<NotorBullet*> *listNotorBullet = &NotorBullet::listNotorBullet;
	cout << "Notor Bullet: " << listNotorBullet->size() << endl;
	for (std::vector<NotorBullet*>::iterator it = listNotorBullet->begin(); it != listNotorBullet->end(); it++)
	{
		if ((*it)->bDisable)
		{
			(*it)->Re_Initialize(GetPosition().x, GetPosition().y, direction);
			return;
		}
	}
	NotorBullet* bullet1 = new NotorBullet();
	bullet1->Ghost_Initialize(GetPosition().x, GetPosition().y, direction);
}

void HeadHunter::FireRocket()
{
	std::vector<Rocket*> *listRocket = &Rocket::listRocket;

	cout << "ROCKET : " << listRocket->size() << endl;

	for (std::vector<Rocket*>::iterator it = listRocket->begin(); it != listRocket->end(); it++)
	{
		if ((*it)->bDisable)
		{
			(*it)->Re_Initialize(GetPosition().x, GetPosition().y, direction);
			return;
		}
	}
	Rocket* bullet1 = new Rocket();
	bullet1->Ghost_Initialize(GetPosition().x, GetPosition().y, direction);
}

void HeadHunter::FireBulletUpdate(float deltatime)
{
	float direct = this->GetPosition().x - Megaman::getInstance()->GetPosition().x;
	if (direction == eDirection::Left)
	{
		if (direct < 0)
		{
			return;
		}
	}
	else
	{
		if (direct > 0)
			return;
	}

	if (state != eHeadHunterState::FireBullet)
		return;

	if (fireTimesCount >= 3)
		return;

	waitForFireTimeCount -= deltatime;
	if (waitForFireTimeCount > 0)
		return;

	fireIntevalCount -= deltatime;

	if (fireIntevalCount < 0)
	{
		fireIntevalCount = fireInteval;
		FireBullet();
		fireTimesCount++;
	}
}

void HeadHunter::FireRocketUpdate(float deltatime)
{
	float direct = this->GetPosition().x - Megaman::getInstance()->GetPosition().x;
	if (direction == eDirection::Left)
	{
		if (direct < 0)
		{
			return;
		}
	}
	else
	{
		if (direct > 0)
			return;
	}

	if (state != eHeadHunterState::FireRocket)
		return;

	if (fireTimesCount >= 2)
		return;

	waitForFireTimeCount -= deltatime;
	if (waitForFireTimeCount > 0)
		return;

	fireIntevalCount -= deltatime;

	if (fireIntevalCount < 0)
	{
		fireIntevalCount = fireInteval;
		FireRocket();
		fireTimesCount++;
	}
}

void HeadHunter::ResetFireBulletTime()
{
	waitForFireTimeCount = waitForFireTime;
	fireIntevalCount = 0;
	fireTimesCount = 0;
}

void HeadHunter::ResetRocketTime()
{
	waitForFireTimeCount = waitForFireTime;
	fireIntevalCount = 0;
	fireTimesCount = 0;
}

bool HeadHunter::IsDisable()
{
	return bDisable;
}

void HeadHunter::Disable()
{
	bDisable = true;
	this->box.Disable();
}

void HeadHunter::Enable()
{
	bDisable = false;
	this->box.Enable();
}

void HeadHunter::Initialize()
{
	Creature::Initialize();
	box.DynamicInitialize(this, 40, 32);
	box.SetPivot(20, 16);
	sprite.get()->SetAnimation("headgunner");
	InitialzieHPComponent(3, 1);

	changeStateTime = 3.f;
	changeStateTimeCount = changeStateTime;

	waitForFireTime = 0.3f;
	waitForFireTimeCount = waitForFireTime;
	fireInteval = 0.5;
	fireIntevalCount = 0;
	fireTimesCount = 0;
	timeToDead = 0.5;
	timeToDeadCount = timeToDead;

	listHeadHunter.push_back(this);
}

void HeadHunter::ReInitialize(D3DXVECTOR2 pos, int direction)
{
	Enable();
	InitialzieHPComponent(3, 1);
	changeStateTimeCount = changeStateTime;

	waitForFireTimeCount = waitForFireTime;
	timeToDeadCount = timeToDead;

	this->SetPosition(pos);

	if (!direction)
	{
		sprite.get()->FlipLeft();
	}
	if (direction)
		this->direction = eDirection::Left;
	else
		this->direction = eDirection::Right;
}

void HeadHunter::UpdateInput(float deltatime)
{
	
}

void HeadHunter::Update(float deltatime)
{
	if (GetHPComponent()->IsDead() && state != eHeadHunterState::onHHDead)
	{
		state = eHeadHunterState::onHHDead;
		sprite.get()->SetAnimation("explosive_1", false);
		GetMoveComponent()->IdleX();
		GetMoveComponent()->IdleY();
	}
	UpdateState(deltatime);

	OnCollision(deltatime);

	//UpdateState(deltatime);

	FireBulletUpdate(deltatime);
	FireRocketUpdate(deltatime);

	box.SetPosition();
}

void HeadHunter::UpdateState(float deltatime)
{
	switch (state)
	{
	case eHeadHunterState::FireBullet:

		changeStateTimeCount -= deltatime;
		if (changeStateTimeCount < 0)
		{
			state = eHeadHunterState::FireRocket;
			changeStateTimeCount = changeStateTime;
			ResetRocketTime();
		}


		break;
	case eHeadHunterState::FireRocket:

		changeStateTimeCount -= deltatime;
		if (changeStateTimeCount < 0)
		{
			state = eHeadHunterState::FireBullet;
			changeStateTimeCount = changeStateTime;
			ResetFireBulletTime();
		}
		break;
	case onHHDead:
		timeToDeadCount -= deltatime;
		if (timeToDeadCount < 0)
			Disable();
		break;
	}
}

void HeadHunter::OnCollision(float deltatime)
{
	// Kiểm tra va chạm với đạn của Megaman
	std::vector<NormalBullet*> *listBullet = &NormalBullet::listNormalBullet;
	if (!listBullet->empty())
	{
		for (std::vector<NormalBullet*>::iterator it = listBullet->begin(); it != listBullet->end(); it++)
		{
			if ((*it)->bDisable)
				continue;
			if (!Collision::IsIntersection(this->box.GetBox(), (*it)->box.GetBox()))
				continue;
			(*it)->Disable();
			// Code Ở đây đoạn trừ máu của enemy
			this->GetHPComponent()->DoDamage((*it)->damage, (*it)->bGodMode);
		}
	}
}

void HeadHunter::Draw()
{
	Creature::Draw();
}

void HeadHunter::Destroy()
{
}

void HeadHunter::UpdateAll(float deltatime)
{
	if (PRINT_SIZE && !PRINT_SIZE_ENABLE_OBJECT)
	{
		static float printTimeCount = 0;
		if (printTimeCount >= 1.0)
		{
			cout << "HeadHunter's Object Amount: " << listHeadHunter.size() << endl;
			printTimeCount = 0;
		}
		printTimeCount += deltatime;
	}

	for (std::vector<HeadHunter*>::iterator it = listHeadHunter.begin(); it != listHeadHunter.end(); it++)
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
			for (std::vector<HeadHunter*>::iterator it = listHeadHunter.begin(); it != listHeadHunter.end(); it++)
			{
				if (!(*it)->IsDisable())
					count++;
			}

			cout << "HeadHunter's Object Enable Amount: " << count << "/" << listHeadHunter.size() << endl;
			printTimeCount2 = 0;
		}
		printTimeCount2 += deltatime;
	}
}

void HeadHunter::DrawAll()
{
	for (std::vector<HeadHunter*>::iterator it = listHeadHunter.begin(); it != listHeadHunter.end(); it++)
	{
		if (!(*it)->IsDisable())
			(*it)->Draw();
	}
}

HeadHunter * HeadHunter::CreateHeadHunter(D3DXVECTOR2 pos, int direction)
{
	for (int i = 0; i < listHeadHunter.size(); i++)
	{
		if (listHeadHunter[i]->IsDisable())
		{
			listHeadHunter[i]->ReInitialize(pos, direction);
			return listHeadHunter[i];
		}
	}
	HeadHunter* head = new HeadHunter(pos, direction);
	return head;
}
