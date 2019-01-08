#include "NotorBanger.h"

std::vector<NotorBanger*> NotorBanger::listNotorBanger;

NotorBanger::NotorBanger(D3DXVECTOR2 pos, int direct)
{
	Initialize();
	SetPosition(pos);
	//demTime = 0;
}

void NotorBanger::FireBullet()
{
	std::vector<NotorBullet*> *listNotorBullet = &NotorBullet::listNotorBullet;

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

void NotorBanger::FireBulletUpdate(float deltatime)
{
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

void NotorBanger::ResetFireTime()
{
	waitForFireTimeCount = waitForFireTime;
	fireIntevalCount = 0;
	fireTimesCount = 0;
}

bool NotorBanger::IsDisable()
{
	return bDisable;
}

void NotorBanger::Disable()
{
	box.Disable();
	bDisable = true;
	bCanReInit = true;
}

void NotorBanger::Disable(int o)
{
	box.Disable();
	bDisable = true;
	bCanReInit = false;
}

void NotorBanger::Enable()
{
	bDisable = false;
	box.Enable();
}

void NotorBanger::Initialize()
{
	Creature::Initialize();
	//GetMoveComponent()->EnableGravity();
	GetMoveComponent()->SetSpeed(150);
	GetMoveComponent()->SetJumpPower(80);
	
	InitialzieHPComponent(3, 1);

	bJumping = false;
	state = eNotorBangerState::Jump;
	fire1Time = 2.0f;
	fire1TimeCount = fire1Time;
	box.DynamicInitialize(this, 40, 32);
	box.SetPivot(20, 16);
	sprite.get()->SetAnimation("notorbanger_change_posture_1");

	//Setting Fire Bullet
	waitForFireTime = 0.3f;
	waitForFireTimeCount = waitForFireTime;
	fireInteval = 0.5;
	fireIntevalCount = 0;
	fireTimesCount = 0;
	timeToDead = 0.5;
	timeToDeadCount = timeToDead;

	listNotorBanger.push_back(this);
}

void NotorBanger::ReInitialize(D3DXVECTOR2 pos, int direct)
{
	GetMoveComponent()->DisableGravity();
	bJumping = false;
	InitialzieHPComponent(3, 1);
	state = eNotorBangerState::Jump;
	fire1TimeCount = fire1Time;
	sprite.get()->SetAnimation("notorbanger_change_posture_1");
	waitForFireTimeCount = waitForFireTime;
	timeToDeadCount = timeToDead;

	this->SetPosition(pos);

	this->Enable();
}

void NotorBanger::GetDirection()
{
	float x = Megaman::getInstance()->GetPosition().x;
	if (x > GetPosition().x)
		direction = eDirection::Right;
	else
		direction = eDirection::Left;
}

void NotorBanger::UpdateInput(float deltatime)
{
}

void NotorBanger::Update(float deltatime)
{
	if (GetHPComponent()->IsDead() && state != eNotorBangerState::onNBDead)
	{
		state = eNotorBangerState::onNBDead;
		sprite.get()->SetAnimation("explosive_1", false);
		GetMoveComponent()->IdleX();
		GetMoveComponent()->IdleY();
	}
	UpdateState(deltatime);
	if (state == eNotorBangerState::onNBDead)
		return;

	OnCollision(deltatime);

	/*demTime += deltatime;
	cout << demTime << endl;*/

	GetMoveComponent()->UpdateMovement(deltatime);
	

	FireBulletUpdate(deltatime);

	box.SetPosition();
}

void NotorBanger::UpdateState(float deltatime)
{
	switch (state)
	{
	case Jump:
		if (bJumping)
		{
			if (direction == eDirection::Left)
				GetMoveComponent()->MoveLeft();
			else
				GetMoveComponent()->MoveRight();
			break;
		}
		demTime = 0;
		GetMoveComponent()->IdleX();
		bFireStyle = !bFireStyle;
		bFireStyle = true;
		ResetFireTime();

		if (bFireStyle)
		{
			GetMoveComponent()->EnableGravity();
			bFire1 = true;
			state = eNotorBangerState::Fire1;
			sprite.get()->SetAnimation("notorbanger_change_posture_1",false);
		}
		else
		{
			GetMoveComponent()->EnableGravity();
			bFire2 = true;
			state = eNotorBangerState::Fire2;
			sprite.get()->SetAnimation("notorbanger_change_posture_2");
		}

		break;
	case Fire1:
		if (bFire1)
		{
			if (fire1TimeCount <= 0)
			{
				bFire1 = false;
				fire1TimeCount = fire1Time;
			}
			else
			{
				fire1TimeCount -= deltatime;
				//std::cout << fire1TimeCount << endl;
				GetDirection();
				if (direction == eDirection::Left)
					sprite.get()->FlipRight();
				else
					sprite.get()->FlipLeft();
			}
			break;
		}
		
		GetMoveComponent()->Jump();
		bJumping = true;
		state = eNotorBangerState::Jump;
		sprite.get()->SetAnimation("notorbanger_jump");
		break;
	case Fire2:
		if (bFire2)
			break;
		state = eNotorBangerState::Jump;
		sprite.get()->SetAnimation("notorbanger_jump");
		break;
	case onNBDead:
		timeToDeadCount -= deltatime;
		if (timeToDeadCount < 0)
		{
			Item::CreateItem(this->GetPosition(), 0);
			Disable(1);
		}
		break;
	default:
		break;
	}
}

void NotorBanger::WallCollision(float collideTime, int normalX, int normalY, float deltatime, Box box)
{
	//Va chạm với Wall và Ground
	if (collideTime >= 0 || collideTime < 0.1)
	{
		/*if (collideTime < 0.1)
		collideTime = 0;*/
		float vecX = GetMoveComponent()->GetVelocity().x;
		float vecY = GetMoveComponent()->GetVelocity().y;
		if (normalX != 0)
		{
			vecX = collideTime * GetMoveComponent()->GetVelocity().x;// *0.1;
		}

		if (normalY > 0)
		{
			vecY = collideTime * GetMoveComponent()->GetVelocity().y;// *0.1;
																	 //if (collideTime != 0)
																	 //cout << "Va cham ben tren" << endl;
		}

		if (normalY < 0)
		{
			this;
			if (collideTime >= 0.01f)
				//vecY = collideTime * GetMoveComponent()->GetVelocity().y;// *0.1;
				vecY = 0;
			else if (collideTime < 0.01f)
			{
				vecY = 0;
			}

			if (Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), box))
				GetPosition_Ptr()->y -= vecY;

			bJumping = false;
			

			if (collideTime != 0)
			{
				//cout << "CollideTime: " << collideTime << "\tVeclocity: " << vecY << "\tPosition: " << GetPosition().y - 16 << endl;
				//cout << "Va cham ben duoi" << endl;
			}
		}

		GetMoveComponent()->SetVelocity(vecX, vecY);
	}
}

void NotorBanger::OnCollision(float deltatime)
{
	// Kiểm tra va chạm với Ground
	std::vector<MapCollision*> *listMapCollsion = &MapCollision::listMapCollision;
	if (!listMapCollsion->empty())
	{
		for (std::vector<MapCollision*>::iterator it = listMapCollsion->begin(); it != listMapCollsion->end(); it++)
		{

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), (*it)->box.GetBox()))
				continue;
			int normalX = 0;
			int normalY = 0;
			float collideTime = Collision::GetCollideTime(this->box, (*it)->box, &normalX, &normalY, deltatime);

			WallCollision(collideTime, normalX, normalY, deltatime, (*it)->box.GetBox());
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

void NotorBanger::Draw()
{
	Creature::Draw();
}

void NotorBanger::Destroy()
{
}

void NotorBanger::UpdateAll(float deltatime)
{
	if (PRINT_SIZE && !PRINT_SIZE_ENABLE_OBJECT)
	{
		static float printTimeCount = 0;
		if (printTimeCount >= 1.0)
		{
			cout << "NotorBanger's Object Amount: " << listNotorBanger.size() << endl;
			printTimeCount = 0;
		}
		printTimeCount += deltatime;
	}

	for (std::vector<NotorBanger*>::iterator it = listNotorBanger.begin(); it != listNotorBanger.end(); it++)
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
			for (std::vector<NotorBanger*>::iterator it = listNotorBanger.begin(); it != listNotorBanger.end(); it++)
			{
				if (!(*it)->IsDisable())
					count++;
			}

			cout << "NotorBanger's Object Enable Amount: " << count << "/" << listNotorBanger.size() << endl;
			printTimeCount2 = 0;
		}
		printTimeCount2 += deltatime;
	}
}

void NotorBanger::DrawAll()
{
	for (std::vector<NotorBanger*>::iterator it = listNotorBanger.begin(); it != listNotorBanger.end(); it++)
	{
		if (!(*it)->IsDisable())
			(*it)->Draw();
	}
}

NotorBanger* NotorBanger::CreateNotorBanger(D3DXVECTOR2 pos, int direct)
{
	for (int i = 0; i < listNotorBanger.size(); i++)
	{
		if (listNotorBanger[i]->IsDisable() && listNotorBanger[i]->bCanReInit)
		{
			listNotorBanger[i]->ReInitialize(pos, direct);
			return listNotorBanger[i];
		}
	}
	NotorBanger* notor = new NotorBanger(pos, direct);
	return notor;
}
