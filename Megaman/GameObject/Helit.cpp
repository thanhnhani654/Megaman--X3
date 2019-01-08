#include "Helit.h"

std::vector<Helit*> Helit::listHelit;

Helit::Helit(D3DXVECTOR2 pos, int direction)
{
	bDisable = false;
	Initialize();
	subSprite.SetAnimation("helit_fan");
	SetPosition(pos);
	if (!direction)
	{
		sprite.get()->FlipLeft();
	}
}

void Helit::GoDownFindPlayer(float deltatime)
{
	if (state != eHelitState::GoDown)
		return;

	GetMoveComponent()->MoveDown();

}

void Helit::RunAway()
{
	if (state != eHelitState::GoUp)
		return;
	GetMoveComponent()->MoveUp();
}

void Helit::MadUpdate(float deltatime)
{
	if (state != eHelitState::Mad)
		return;

	if (madstage < 2)
	{
		madTime1Count -= deltatime;
		if (madTime1Count < 0)
		{
			madTime1Count = madTime1;
			madstage++;
		}
	}

	if (madstage >= 2)
	{
		madTime2Count -= deltatime;
		if (madTime2Count < 0)
		{
			madTime2Count = madTime2 - deltatime;
			madstage++;
		}
	}

	switch (madstage)
	{
	case 0:
		this->GetMoveComponent()->MoveUp();
		break;
	case 1:
		this->GetMoveComponent()->MoveDown();
		break;
	case 2:
		this->GetMoveComponent()->MoveUp();
		break;
	case 3:
		/*this->GetMoveComponent()->MoveDown();
		if (this->GetPosition().y == tempY)
			ResetMadTime();*/
		ResetMadTime();
		state = eHelitState::GoDown;
		break;
	case 4:
		ResetMadTime();
		break;
	}
}

void Helit::ResetMadTime()
{
	madTime1Count = madTime1;
	madTime2Count = madTime2;
	madstage = 0;
}

void Helit::FireRocket()
{
	fireCount++;

	std::vector<HelitRocket*> *listHelitRocket = &HelitRocket::listHelitRocket;

	for (std::vector<HelitRocket*>::iterator it = listHelitRocket->begin(); it != listHelitRocket->end(); it++)
	{
		if ((*it)->bDisable)
		{
			(*it)->Re_Initialize(GetPosition().x, GetPosition().y, direction);
			return;
		}
	}
	HelitRocket* bullet1 = new HelitRocket();
	bullet1->Ghost_Initialize(GetPosition().x, GetPosition().y, direction);
}

bool Helit::IsDisable()
{
	return bDisable;
}

void Helit::Disable()
{
	bDisable = true;
	this->box.Disable();
}

void Helit::Enable()
{
	bDisable = false;
	this->box.Enable();
}

void Helit::Initialize()
{
	Creature::Initialize();
	box.DynamicInitialize(this, 40, 32);
	box.SetPivot(20, 16);
	sprite.get()->SetAnimation("helit");
	state = eHelitState::GoDown;

	GetMoveComponent()->DisableGravity();
	GetMoveComponent()->SetSpeed(120);
	InitialzieHPComponent(3, 1);

	fireTimeInterval = 1.f;
	fireTimeIntervalCount = fireTimeInterval;
	fireCount = 0;
	timeToDead = 0.5;
	timeToDeadCount = timeToDead;

	madTime1 = 0.3f;
	madTime1Count = madTime1;
	madTime2 = 0.5f;
	madTime2Count = madTime2;
	madstage = 0;

	bMad = false;

	listHelit.push_back(this);
}

void Helit::ReInitialize(D3DXVECTOR2 pos, int direction)
{
	InitialzieHPComponent(3, 1);
	sprite.get()->SetAnimation("helit");
	state = eHelitState::GoDown;
	fireTimeIntervalCount = fireTimeInterval;
	madTime1Count = madTime1;
	madTime2Count = madTime2;
	timeToDeadCount = timeToDead;
	bMad = false;
}

void Helit::UpdateInput(float deltatime)
{
}

void Helit::Update(float deltatime)
{
	if (GetHPComponent()->IsDead() && state != eHelitState::onHDead)
	{
		state = eHelitState::onHDead;
		sprite.get()->SetAnimation("explosive_1", false);
		GetMoveComponent()->IdleX();
		GetMoveComponent()->IdleY();
	}
	UpdateState(deltatime);

	OnCollision(deltatime);

	GetMoveComponent()->UpdateMovement(deltatime);
	//UpdateState(deltatime);

	GoDownFindPlayer(deltatime);
	RunAway();
	MadUpdate(deltatime);

	box.SetPosition();
	bMad = false;
	OnCollision(deltatime);
}

void Helit::UpdateState(float deltatime)
{
	switch (state)
	{
	case GoDown:
		if (this->GetPosition().y <= Megaman::getInstance()->GetPosition().y)
		{
			GetMoveComponent()->IdleY();
			state = eHelitState::Fire;
			break;
		}

		if (bMad)
		{
			GetMoveComponent()->IdleY();
			state = eHelitState::Mad;
			ResetMadTime();
		}
		break;
	case GoUp:
		break;
	case Mad:
		if (this->GetPosition().y <= Megaman::getInstance()->GetPosition().y && !bMad)
		{
			GetMoveComponent()->IdleY();
			state = eHelitState::Fire;
			break;
		}
		break;
	case Fire:
		if (fireCount >= 2)
		{
			state = eHelitState::GoUp;
			break;
		}

		if (bMad)
		{
			GetMoveComponent()->IdleY();
			state = eHelitState::Mad;
			ResetMadTime();
		}

		if (this->GetPosition().y > Megaman::getInstance()->GetPosition().y)
		{
			state = eHelitState::GoDown;
			break;
		}

		fireTimeIntervalCount -= deltatime;

		if (fireTimeIntervalCount < 0)
		{
			FireRocket();
			fireTimeIntervalCount = fireTimeInterval;
		}
		break;
	case onHDead:
		timeToDeadCount -= deltatime;
		if (timeToDeadCount < 0)
			Disable();
		break;
	default:
		break;
	}
}

void Helit::OnCollision(float deltatime)
{
	if (state == eHelitState::GoUp)
		return;

	std::vector<MapCollision*> *listMapCollsion = &MapCollision::listMapCollision;
	if (!listMapCollsion->empty())
	{
		for (std::vector<MapCollision*>::iterator it = listMapCollsion->begin(); it != listMapCollsion->end(); it++)
		{

			if (!Collision::IsIntersection(this->box.GetBox(), (*it)->box.GetBox()))
				continue;
			
			//state = eHelitState::Mad;
			tempY = this->GetPosition().y;
			bMad = true;
		}
	}

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

void Helit::Draw()
{
	Creature::Draw();
	subSprite.Render(this->GetPosition().x, this->GetPosition().y + 20);
}

void Helit::Destroy()
{
}

void Helit::UpdateAll(float deltatime)
{
	if (PRINT_SIZE && !PRINT_SIZE_ENABLE_OBJECT)
	{
		static float printTimeCount = 0;
		if (printTimeCount >= 1.0)
		{
			cout << "Helit's Object Amount: " << listHelit.size() << endl;
			printTimeCount = 0;
		}
		printTimeCount += deltatime;
	}

	for (std::vector<Helit*>::iterator it = listHelit.begin(); it != listHelit.end(); it++)
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
			for (std::vector<Helit*>::iterator it = listHelit.begin(); it != listHelit.end(); it++)
			{
				if (!(*it)->IsDisable())
					count++;
			}

			cout << "Helit's Object Enable Amount: " << count << "/" << listHelit.size() << endl;
			printTimeCount2 = 0;
		}
		printTimeCount2 += deltatime;
	}
}

void Helit::DrawAll()
{
	for (std::vector<Helit*>::iterator it = listHelit.begin(); it != listHelit.end(); it++)
	{
		if (!(*it)->IsDisable())
			(*it)->Draw();
	}
}

Helit * Helit::CreateHelit(D3DXVECTOR2 pos, int direction)
{
	for (int i = 0; i < listHelit.size(); i++)
	{
		if (listHelit[i]->IsDisable())
		{
			listHelit[i]->ReInitialize(pos, direction);
			return listHelit[i];
		}
	}
	Helit* helit = new Helit(pos, direction);
	return helit;
}
