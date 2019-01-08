#include "BlastHornet.h"

std::vector<BlastHornet*> BlastHornet::listBlastHornet;

BlastHornet::BlastHornet(D3DXVECTOR2 pos, int direction)
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

bool BlastHornet::IsDisable()
{
	return bDisable;
}

void BlastHornet::Disable()
{
	bDisable = true;
	this->box.Disable();
	bCanReInit = true;
}

void BlastHornet::Disable(int i)
{
	bDisable = true;
	this->box.Disable();
	bCanReInit = false;
}

void BlastHornet::Enable()
{
	bDisable = false;
	this->box.Enable();
}

void BlastHornet::Initialize()
{
	Creature::Initialize();
	box.DynamicInitialize(this, 40, 32);
	box.SetPivot(20, 16);
	sprite.get()->SetAnimation("blasthornet");
	subSprite.SetAnimation("blasthornet_wing");
	state = eBlastHornetState::DetectTarget;
	InitialzieHPComponent(25, 5);
	bStartBossFight = false;

	changeStateTime = 3.f;
	changeStateTimeCount = changeStateTime;
	detectTargetTime = 2.0f;
	detectTargetTimeCount = detectTargetTime;
	attackTime = 1;
	attackTimeCount = attackTime;
	backTime = 1;
	backTimeCount = backTime;
	angleAttackTime = 2;
	angleAttackTimeCount = angleAttackTime;
	timeLine = 3.14/4;
	moveState = eMysteryState::giaidoan1;
	nextMove = 0;
	prevMove = 0;
	timeToDead = 0.5;
	timeToDeadCount = timeToDead;

	listBlastHornet.push_back(this);
}

void BlastHornet::ReInitialize(D3DXVECTOR2 pos, int direction)
{
	Enable();

	changeStateTimeCount = changeStateTime;

	InitialzieHPComponent(30, 5);

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

void BlastHornet::MoveTo(D3DXVECTOR2 itargetLocation, float itime)
{
	float distanceX = itargetLocation.x - this->GetPosition().x;
	float distanceY = itargetLocation.y - this->GetPosition().y;

	if (distanceX < 0)
		sprite.get()->FlipRight();
	else
		sprite.get()->FlipLeft();
	//if (distanceY < 0)
	//	distanceY -= 50;

	float vecX = distanceX / itime;
	float vecY = distanceY / itime;

	GetMoveComponent()->SetVelocity(vecX, vecY);
}

void BlastHornet::MysteryMove(D3DXVECTOR2 itargetLocation)
{
}

void BlastHornet::LayTam()
{
	float distanceX = attackPosition1.x - attackPosition2.x;	//Khoảng cách từ điểm tấn công 1 đến điểm tấn công 2
	distanceX /= 2;
	float a = distanceX / (1 + sqrt(2) / 2);
	float b = a*(sqrt(2) / 2);
	r = a;
	
	point1.x = attackPosition1.x - b;
	point1.y = attackPosition1.y - b;
	
	point2.x = attackPosition2.x + b;
	point2.y = attackPosition2.y - b;

	giaoDiem.x = attackPosition1.x - distanceX;
	giaoDiem.y = attackPosition1.y - b;
}

void BlastHornet::PhuongTrinhChuyenDongTron(float t)
{
	float limitCondition = 1.0f;
	if (point1.x == 0 && point1.y == 0)
		return;
	D3DXVECTOR2 newPos;
	switch (moveState)
	{
	case giaidoan1:
		newPos.x = r*sin(t) + point1.x;
		newPos.y = r*cos(t) + point1.y;

		if (abs(newPos.x - giaoDiem.x) < limitCondition && (abs(newPos.y - giaoDiem.y) < limitCondition))
		{
			newPos.x = giaoDiem.x;
			newPos.y = giaoDiem.y;
			moveState = giaidoan2;
		}

		//cout << giaoDiem.x - newPos.x << "\t\t" << giaoDiem.y - newPos.y << endl;
		break;
	case giaidoan2:
		newPos.x = r*sin(-t) + point2.x;
		newPos.y = r*cos(t) + point2.y;

		if (abs(newPos.x - attackPosition2.x) < limitCondition && (abs(newPos.y - attackPosition2.y) < limitCondition))
		{
			newPos.x = attackPosition2.x;
			newPos.y = attackPosition2.y;
			attackTimeCount = attackTime;
			if (nextMove == 1)
			{
				state = eBlastHornetState::DetectTarget;
				sprite.get()->FlipLeft();
			}
			moveState = giaidoan3;
		}

		//cout << attackPosition2.x - newPos.x << "\t\t" << attackPosition2.y - newPos.y << endl;
		break;
	case giaidoan3:
		newPos.x = r*sin(-t) + point2.x;
		newPos.y = r*cos(t) + point2.y;

		if (abs(newPos.x - giaoDiem.x) < limitCondition && (abs(newPos.y - giaoDiem.y) < limitCondition))
		{
			newPos.x = giaoDiem.x;
			newPos.y = giaoDiem.y;
			attackTimeCount = attackTime;
			moveState = giaidoan4;
		}

		//cout << giaoDiem.x - newPos.x << "\t\t" << giaoDiem.y - newPos.y << endl;
		break;
	case giaidoan4:
		newPos.x = r*sin(t) + point1.x;
		newPos.y = r*cos(t) + point1.y;

		if (abs(newPos.x - attackPosition1.x) < limitCondition && (abs(newPos.y - attackPosition1.y) < limitCondition))
		{
			newPos.x = attackPosition1.x;
			newPos.y = attackPosition1.y;
			attackTimeCount = attackTime;
			if (nextMove == 0)
			{
				state = eBlastHornetState::DetectTarget;
				sprite.get()->FlipRight();
			}
			moveState = giaidoan1;
		}

		//cout << attackPosition1.x - newPos.x << "\t\t" << attackPosition1.y - newPos.y << endl;
		break;
	}
	

	this->SetPosition(newPos);
}

void BlastHornet::RandomNextMove()
{
	prevMove = nextMove;
	nextMove = rand() % 2;
}

void BlastHornet::Fire()
{
	D3DXVECTOR2 target = Megaman::getInstance()->GetPosition();
	Bee* bee = Bee::CreateBee(this->GetPosition(), this->GetDirection());
	bee->MoveTo(target);

	target.x -= 50;
	Bee* bee1 = Bee::CreateBee(this->GetPosition(), this->GetDirection());
	bee1->MoveTo(target);

	target.x -= 50;
	Bee* bee2 = Bee::CreateBee(this->GetPosition(), this->GetDirection());
	bee2->MoveTo(target);

	target.x += 150;
	Bee* bee3 = Bee::CreateBee(this->GetPosition(), this->GetDirection());
	bee3->MoveTo(target);

	target.x += 50;
	Bee* bee4 = Bee::CreateBee(this->GetPosition(), this->GetDirection());
	bee4->MoveTo(target);

}

int BlastHornet::GetDirection()
{
	if (this->GetPosition().x - Megaman::getInstance()->GetPosition().x > 0)
		return 1;
	return 0;
}

void BlastHornet::FaceTarget()
{
	if (GetDirection())
		this->sprite.get()->FlipRight();
	else
		this->sprite.get()->FlipLeft();
}

void BlastHornet::UpdateInput(float deltatime)
{

}

void BlastHornet::Update(float deltatime)
{
	if (!Megaman::getInstance()->bHornetFiglt)
		return;
	Megaman::getInstance()->bEndBossFight = false;

	if (GetHPComponent()->IsDead() && state != eBlastHornetState::onBHDead)
	{
		state = eBlastHornetState::onBHDead;
		sprite.get()->SetAnimation("explosive_1", false);
		GetMoveComponent()->IdleX();
		GetMoveComponent()->IdleY();
	}

	OnCollision(deltatime);

	GetMoveComponent()->UpdateMovement(deltatime);

	UpdateState(deltatime);

	box.SetPosition();

	HUD::getInstance()->GetBossHp(GetHPComponent()->GetHP());
}

void BlastHornet::UpdateState(float deltatime)
{
	static bool getOnce = false;
	switch (state)
	{
	case eBlastHornetState::DetectTarget:

		if (!getOnce)
		{
			getOnce = true;
			attackPosition1 = this->GetPosition();
			attackPosition2 = this->GetPosition();
			attackPosition2.x -= 120;
		}

		detectTargetTimeCount -= deltatime;
		FaceTarget();
		if (detectTargetTimeCount <= 0)
		{
			RandomNextMove();
			detectTargetTimeCount = detectTargetTime;
			state = eBlastHornetState::DirectAttack;
			targetLocation = Megaman::getInstance()->GetPosition();
			
			MoveTo(Megaman::getInstance()->GetPosition(), attackTime);
			sprite.get()->SetAnimation("blasthornet_sting", false);
		}
		break;
	case eBlastHornetState::DirectAttack:
		attackTimeCount -= deltatime;
		if (attackTimeCount < 0)
		{
			attackTimeCount = attackTime;
			GetMoveComponent()->IdleX();
			GetMoveComponent()->IdleY();

			if (prevMove == 0)
				MoveTo(attackPosition1, attackTime);
			else
				MoveTo(attackPosition2, attackTime);
			state = eBlastHornetState::Back;

			sprite.get()->SetAnimation("blasthornet", false);
		}
		break;
	case eBlastHornetState::Back:

		attackTimeCount -= deltatime;
		if (attackTimeCount < 0)
		{
			GetMoveComponent()->IdleX();
			GetMoveComponent()->IdleY();
			if (prevMove == 0)
				sprite.get()->FlipRight();
			else
				sprite.get()->FlipLeft();
		}
		backTimeCount -= deltatime;
		if (backTimeCount < 0)
		{
			backTimeCount = backTime;
			state = eBlastHornetState::AngleAttack; 
			FaceTarget();
			Fire();
		}

		break;
	case eBlastHornetState::AngleAttack:
		
		angleAttackTimeCount -= deltatime;
		FaceTarget();
		if (angleAttackTimeCount < 0)
		{
			angleAttackTimeCount = angleAttackTime;
			LayTam();
			state = eBlastHornetState::HuhuKheckhec;
		}

		break;
	case eBlastHornetState::HuhuKheckhec:
		FaceTarget();
		PhuongTrinhChuyenDongTron(timeLine);
		timeLine += deltatime * 1.5f;
		break;
	case eBlastHornetState::onBHDead:
		timeToDeadCount -= deltatime;
		if (timeToDeadCount < 0)
		{
			//Item::CreateItem(this->GetPosition(), 0);
			Megaman::getInstance()->bEndBossFight = true;
			Disable(1);
		}
		break;
	}
	

}

void BlastHornet::OnCollision(float deltatime)
{
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
			this->GetHPComponent()->DoDamage((*it)->damage, (*it)->bGodMode);
			(*it)->Disable();
			return;
		}
	}
}

void BlastHornet::Draw()
{
	if (!Megaman::getInstance()->bHornetFiglt)
		return;
	subSprite.Render(this->GetPosition().x, this->GetPosition().y+40);
	Creature::Draw();
}

void BlastHornet::Destroy()
{
}

void BlastHornet::UpdateAll(float deltatime)
{
	if (PRINT_SIZE && !PRINT_SIZE_ENABLE_OBJECT)
	{
		static float printTimeCount = 0;
		if (printTimeCount >= 1.0)
		{
			cout << "BlastHornet's Object Amount: " << listBlastHornet.size() << endl;
			printTimeCount = 0;
		}
		printTimeCount += deltatime;
	}

	for (std::vector<BlastHornet*>::iterator it = listBlastHornet.begin(); it != listBlastHornet.end(); it++)
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
			for (std::vector<BlastHornet*>::iterator it = listBlastHornet.begin(); it != listBlastHornet.end(); it++)
			{
				if (!(*it)->IsDisable())
					count++;
			}

			cout << "BlastHornet's Object Enable Amount: " << count << "/" << listBlastHornet.size() << endl;
			printTimeCount2 = 0;
		}
		printTimeCount2 += deltatime;
	}
}

void BlastHornet::DrawAll()
{
	for (std::vector<BlastHornet*>::iterator it = listBlastHornet.begin(); it != listBlastHornet.end(); it++)
	{
		if (!(*it)->IsDisable())
			(*it)->Draw();
	}
}

BlastHornet * BlastHornet::CreateBlastHornet(D3DXVECTOR2 pos, int direction)
{
	for (int i = 0; i < listBlastHornet.size(); i++)
	{
		if (listBlastHornet[i]->IsDisable())
		{
			listBlastHornet[i]->ReInitialize(pos, direction);
			return listBlastHornet[i];
		}
	}
	BlastHornet* head = new BlastHornet(pos, direction);
	return head;
}
