#include "Megaman.h"

void Megaman::Initialize()
{
	Creature::Initialize();
	state = eMegamanState::Stand;
	sprite.get()->SetAnimation("char_stand");	
	GetMoveComponent()->EnableGravity();
	GetMoveComponent()->SetSpeed(150);
	GetMoveComponent()->SetJumpPower(100);
	InitialzieHPComponent(10, 1);

	//Thuộc tính riêng biệt
	limitDashTime = 0.4f;
	dashTime = 0.4f;
	chargeBar = 0;
	chargeLv2 = 1;
	chargeLv3 = 2;
	shootTime = 0.3f;
	shootCountTime = shootTime;
	hurtTime = 1.0f;
	hurtTimeCount = hurtTime;
	immortalTime = hurtTime + 1.0f;
	immortalCountTime = immortalTime;
	hurtEffect = 5;
	hurtEffectCount = hurtEffect;
	timePassGate = 2.0f;
	timePassGateCount = timePassGate;
	waitOpenGateTime = 1.5f;
	waitOpenGateTimeCount = waitOpenGateTime;
	bPassingGate = false;
	bDisableInput = false;

	box.DynamicInitialize(this, 16, 26);
	box.SetPivot(8,16);

	GetTagMethod()->AddTag(eTag::PlayerTag);
	GetHPComponent()->ToggleGodMode();
}

void Megaman::Ghost_Initialize(){}

void Megaman::MovementController(float deltatime)
{
	if (bClingWall)
		return;

	if (IsKeyDown(DIK_D))
	{
		GetMoveComponent()->MoveRight();
		sprite.get()->FlipRight();
		bMoving = true;
		direction = eDirection::Right;
	}
	else if (IsKeyDown(DIK_A))
	{
		GetMoveComponent()->MoveLeft();
		sprite.get()->FlipLeft();
		bMoving = true;
		direction = eDirection::Left;
	}
	else
	{
		GetMoveComponent()->IdleX();
		bMoving = false;
	}

	if (IsKeyDown(DIK_L))
	{
		if (dashTime > 0)
		{
			if (direction == eDirection::Right)
				GetMoveComponent()->Dash(0);
			else
				GetMoveComponent()->Dash(1);
			GetMoveComponent()->OnDashing();
		}
		else
		{
			if (bClingWall)
			{
				bDash = false;
				GetMoveComponent()->OffDashing();
			}
			if (!bJump)
			{
				GetMoveComponent()->OffDashing();
				bDash = false;
				state = eMegamanState::Stand;
			}
			if (!bMoving)
			{
				GetMoveComponent()->IdleX();	
				bDash = false;
				state = eMegamanState::Jump;
			}
		}
		bDash = true;
		if (bDash)
		{
			dashTime -= deltatime;
		}
	}


}

void Megaman::GroundCollision(float collideTime, int normalX, int normalY, float deltatime, Box box)
{
	//Va chạm với Wall và Ground
	if (collideTime >= 0 || collideTime < 0.1)
	{
		float vecX = GetMoveComponent()->GetVelocity().x;
		float vecY = GetMoveComponent()->GetVelocity().y;
		if (normalX != 0)
		{
			vecX = collideTime * GetMoveComponent()->GetVelocity().x;
		}

		if (normalY > 0)
		{
			vecY = collideTime * GetMoveComponent()->GetVelocity().y;
		}

		if (normalY < 0)
		{
			this;
			if (collideTime >= 0.01f)
				vecY = 0;
			else if (collideTime < 0.01f)
			{
				vecY = 0;
			}

			if (Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), box))
				GetPosition_Ptr()->y -= vecY;

			bJump = false;
			if (collideTime != 0)
			{
				//cout << "CollideTime: " << collideTime << "\tVeclocity: " << vecY << "\tPosition: " << GetPosition().y - 16 << endl;
				//cout << "Va cham ben duoi" << endl;
			}
		}

		GetMoveComponent()->SetVelocity(vecX, vecY);
	}
}

/*
void Megaman::LeftUpGroundCollision(float collideTime, int normalX, int normalY, float deltatime, Box box)
{
	//Va chạm với Wall và Ground
	if (collideTime >= 0 || collideTime < 0.1)
	{
		float vecX = GetMoveComponent()->GetVelocity().x;
		float vecY = GetMoveComponent()->GetVelocity().y;
		if (normalX > 0)
		{
			//vecX = collideTime * GetMoveComponent()->GetVelocity().x;
			int a = 0;
			float temp =  box.position.y + box.size.y - (this->box.GetPosition().y - this->box.GetSize().y) + 1;
			this->GetPosition_Ptr()->y += temp;
			this->box.SetPosition();
		}
		if (normalX < 0)
		{
			vecX = collideTime * GetMoveComponent()->GetVelocity().x;
		}

		if (normalY > 0)
		{
			vecY = collideTime * GetMoveComponent()->GetVelocity().y;
		}

		if (normalY < 0)
		{
			this;
			if (collideTime >= 0.01f)
				vecY = 0;
			else if (collideTime < 0.01f)
			{
				vecY = 0;
			}

			if (Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), box))
				GetPosition_Ptr()->y -= vecY;

			bJump = false;
			if (collideTime != 0)
			{
				//cout << "CollideTime: " << collideTime << "\tVeclocity: " << vecY << "\tPosition: " << GetPosition().y - 16 << endl;
				//cout << "Va cham ben duoi" << endl;
			}
		}

		GetMoveComponent()->SetVelocity(vecX, vecY);
	}
}*/

void Megaman::WallCollision(float collideTime, int normalX, int normalY, float deltatime, Box box)
{
	if (collideTime >= 0 || collideTime < 0.1)
	{
		float vecX = GetMoveComponent()->GetVelocity().x;
		float vecY = GetMoveComponent()->GetVelocity().y;
		if (normalX != 0)
		{
			vecX = collideTime * GetMoveComponent()->GetVelocity().x;
		}
		GetMoveComponent()->SetVelocity(vecX, vecY);
		if (bJump && !bClingWall)
		{
			bJump = false;
			bClingWall = true;
			GetMoveComponent()->IdleX();		
			GetMoveComponent()->DisableGravity();
			GetMoveComponent()->IdleY();
		}
		
		
	}
}

void Megaman::ElevatorCollision(float collideTime, int normalX, int normalY, float deltatime, Box box, float eSpeed)
{
	//Va chạm với Wall và Ground
	if (collideTime >= 0 || collideTime < 0.1)
	{
		float vecX = GetMoveComponent()->GetVelocity().x;
		float vecY = GetMoveComponent()->GetVelocity().y;
		if (normalX != 0)
		{
			vecX = collideTime * GetMoveComponent()->GetVelocity().x;
		}

		if (normalY > 0)
		{
			vecY = collideTime * GetMoveComponent()->GetVelocity().y;
		}

		if (normalY < 0)
		{
			this;
			if (collideTime >= 0.01f)
				vecY = 0;
			else if (collideTime < 0.01f)
			{
				vecY = 0;
			}

			if (Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), box))
				GetPosition_Ptr()->y -= vecY;

			bJump = false;
			if (collideTime != 0)
			{
				//cout << "CollideTime: " << collideTime << "\tVeclocity: " << vecY << "\tPosition: " << GetPosition().y - 16 << endl;
				//cout << "Va cham ben duoi" << endl;
			}
			vecY = eSpeed;
		}
		GetMoveComponent()->SetVelocity(vecX, vecY);
	}
}

void Megaman::GateCollision(float collideTime, int normalX, int normalY, float deltatime, Box box, float eSpeed)
{
	if (collideTime >= 0 || collideTime < 0.1)
	{
		
		float vecX = GetMoveComponent()->GetVelocity().x;
		float vecY = GetMoveComponent()->GetVelocity().y;
		if (normalX != 0)
		{
			vecX = collideTime * GetMoveComponent()->GetVelocity().x;
		}
		GetMoveComponent()->SetVelocity(vecX, vecY);
		if (bJump && !bClingWall)
		{
			bJump = false;
			bClingWall = true;
			GetMoveComponent()->IdleX();
			GetMoveComponent()->DisableGravity();
			GetMoveComponent()->IdleY();
		}
	}
}

void Megaman::EnemyCollision(int direction)
{
	bImmortal = true;
	immortalCountTime = immortalTime;
	cout << "dau" << endl;
	GetMoveComponent()->IdleX();
	GetMoveComponent()->IdleY();

	float vecX = 0;
	float vecY = 0;

	if (direction > 0)
		//GetMoveComponent()->TinhVanTocDuaTrenPhuongTrinhTheoThoiGian(50, 250);
		vecX = 50;
	else
		//GetMoveComponent()->TinhVanTocDuaTrenPhuongTrinhTheoThoiGian(-50, 250);
		vecX = -50;
	vecY = 250;
	GetMoveComponent()->SetVelocity(vecX, vecY);
}

void Megaman::FireBullet()
{
	//Hình như direction của bullet bị lỗi
	std::vector<NormalBullet*> *listNormalBullet = &NormalBullet::listNormalBullet;
	
	for (std::vector<NormalBullet*>::iterator it = listNormalBullet->begin(); it != listNormalBullet->end(); it++)
	{
		if ((*it)->bDisable)
		{
			if (chargeBar < 1)
				(*it)->Re_Initialize(GetPosition().x, GetPosition().y, direction,1, GetHPComponent()->GetDamage(), GetHPComponent()->IsGodMode());
			else if (chargeBar < 2 && chargeBar >= 1)
				(*it)->Re_Initialize(GetPosition().x, GetPosition().y, direction, 2, GetHPComponent()->GetDamage(), GetHPComponent()->IsGodMode());
			else
				(*it)->Re_Initialize(GetPosition().x, GetPosition().y, direction, 3, GetHPComponent()->GetDamage(), GetHPComponent()->IsGodMode());
			chargeBar = 0;
			bShoot = true;
			return;
		}
	}
	NormalBullet* bullet1 = new NormalBullet();
	if (chargeBar < 1)
		bullet1->Ghost_Initialize(GetPosition().x, GetPosition().y, direction, 1, GetHPComponent()->GetDamage(), GetHPComponent()->IsGodMode());
	else if (chargeBar < 2 && chargeBar >= 1)
		bullet1->Ghost_Initialize(GetPosition().x, GetPosition().y, direction, 2, GetHPComponent()->GetDamage(), GetHPComponent()->IsGodMode());
	else
		bullet1->Ghost_Initialize(GetPosition().x, GetPosition().y, direction, 3, GetHPComponent()->GetDamage(), GetHPComponent()->IsGodMode());
	chargeBar = 0;
	bShoot = true;

}

void Megaman::ChargeUp(float deltatime)
{
	if (IsKeyDown(DIK_J))
	{
		chargeBar += deltatime*2;
		if (chargeBar >= 1 && chargeBar < 2)
			std::cout << "Charge LV2" << endl;
		else if (chargeBar >= 2)
			std::cout << "Charge LV3" << endl;
	}
}

void Megaman::Jump()
{
	if (!bJump)
	{
		if (bClingWall)
			bClingWall = false;
		GetMoveComponent()->EnableGravity();
		GetMoveComponent()->Jump();
		bJump = true;
	}
}

void Megaman::ShootCountDown(float deltatime)
{
	if (bShoot)
	{
		if (shootCountTime <= 0)
		{
			bShoot = false;
			shootCountTime = shootTime;
		}
		else
			shootCountTime -= deltatime;
	}
}

void Megaman::HurtCountDown(float deltatime)
{
	immortalCountTime -= deltatime;
	if (immortalCountTime < 0)
		bImmortal = false;
	if (state != eMegamanState::Hurt)
		return;
	hurtTimeCount -= deltatime;
	if (hurtTimeCount > 0)
		return;
	hurtTimeCount = hurtTime;
	state = eMegamanState::Stand;
}

void Megaman::PassGate(float deltatime)
{
	if (!bPassingGate)
		return;
	if (timePassGateCount <= 0)
	{
		timePassGateCount = timePassGate;
		waitOpenGateTimeCount = waitOpenGateTime;
		sprite.get()->ToggleAnimationClip();
		bPassingGate = false;
		bDisableInput = false;
		if (tempGate == nullptr)
			cout << "ERRORRRRRRR: tempGate null" << endl;
		else
			tempGate->GateClosed();
	}

	if (waitOpenGateTimeCount <= 0)
	{
		GetMoveComponent()->SetVelocity(30, GetMoveComponent()->GetVelocity().y);
		timePassGateCount -= deltatime;
	}
	waitOpenGateTimeCount -= deltatime;

}

void Megaman::UpdateInput(float deltatime)
{
	//if (!bDisableInput)
		_ProcessKeyBoard();
	if (state != eMegamanState::Hurt)
	{
		if (!bDisableInput)
		{
			MovementController(deltatime);
			ChargeUp(deltatime);
		}
	}

	//Trước khi kết thúc cập nhật thì cập nhật lại trạng thái 
	UpdateState(deltatime);
	this->box.SetPosition();

	OnCollision(deltatime);
}

void Megaman::Update(float deltatime)
{
	PassGate(deltatime);
	//Bắt buộc để MoveComponent hoạt động
	GetMoveComponent()->UpdateMovement(deltatime);

	UpdateBullet(deltatime);
	ShootCountDown(deltatime);
	HurtCountDown(deltatime);
	
}

void Megaman::UpdateBullet(float deltatime)
{
	std::vector<NormalBullet*>* listNormalBullet = &NormalBullet::listNormalBullet;
	for (std::vector<NormalBullet*>::iterator it = listNormalBullet->begin(); it != listNormalBullet->end(); it++)
	{
		if (!(*it)->bDisable)
			(*it)->Update(deltatime);
	}
}

void Megaman::UpdateState(float deltatime)
{
	// Tạm thời chưa bỏ bDash vào vì không biết animation nằm đâu
	switch (state)
	{
	case eMegamanState::Stand:
		if (bMoving)
			state = eMegamanState::Run;
		else if (bShoot)
			state = eMegamanState::StandShoot;
		else if (bJump)
			state = eMegamanState::Jump;
		else if (bDash)
			state = eMegamanState::Dash;
		break;
	case eMegamanState::StandShoot:
		if (!bShoot)
			state = eMegamanState::Stand;
		else if (bMoving)
			state = eMegamanState::RunShoot;
		else if (bJump)
			state = eMegamanState::JumpShoot;
		else if (bDash)
			state = eMegamanState::Dash;
		break;
	case eMegamanState::Run:
		if (bShoot)
			state = eMegamanState::RunShoot;
		else if (!bMoving)
			state = eMegamanState::Stand;
		else if (bJump)
			state = eMegamanState::Jump;
		else if (bDash)
			state = eMegamanState::Dash;
		break;
	case eMegamanState::RunShoot:
		if (!bShoot)
			state = eMegamanState::Run;
		else if (!bMoving)
			state = eMegamanState::StandShoot;
		else if (bJump)
			state = eMegamanState::JumpShoot;
		else if (bDash)
			state = eMegamanState::Dash;
		break;
	case eMegamanState::Jump:
		if (bShoot)
			state = eMegamanState::JumpShoot;
		else if (bClingWall)
			state = eMegamanState::ClingWall;
		else if (!bJump)
			state = eMegamanState::Stand;
		
		break;
	case eMegamanState::JumpShoot:
		if (!bShoot)
			state = eMegamanState::Jump;
		else if (bClingWall)
			state = eMegamanState::ClingWall;
		else if (!bJump)
			state = eMegamanState::Stand;
		
		break;
	case eMegamanState::Dash:
		if (bJump)
			state = eMegamanState::Jump;
		else if (!bDash)
			state = eMegamanState::Stand;
		break;
	case eMegamanState::ClingWall:
		if (bJump)
			state = eMegamanState::Jump;
		break;
	case eMegamanState::Hurt:
		break;
	default:
		break;
	}
	if (prevState != state)
	{
		UpdateAnimation();
		prevState = state;
	}
}

void Megaman::UpdateAnimation()
{
	switch (state)
	{
	case eMegamanState::Stand:
		sprite.get()->SetAnimation("char_stand");
		break;
	case eMegamanState::StandShoot:
		sprite.get()->SetAnimation("char_stand_shoot");
		break;
	case eMegamanState::Run:
		sprite.get()->SetAnimation("char_run");
		break;
	case eMegamanState::RunShoot:
		sprite.get()->SetAnimation("char_run_shoot");
		break;
	case eMegamanState::Jump:
		sprite.get()->SetAnimation("char_jump", false);
		break;
	case eMegamanState::JumpShoot:
		sprite.get()->SetAnimation("char_jump_shoot", false);
		break;
	case eMegamanState::Dash:
		sprite.get()->SetAnimation("char_dash", false);
		break;
	case eMegamanState::ClingWall:
		sprite.get()->SetAnimation("char_cling_wall", false);
		break;
	case eMegamanState::Hurt:
		sprite.get()->SetAnimation("char_injured", false);
		break;
	default:
		break;
	}
}

void Megaman::Draw()
{
	if (!bImmortal)
		Creature::Draw();
	else
		if (hurtEffectCount <= 0)
		{
			Creature::Draw();
			hurtEffectCount = hurtEffect;
		}
		else
			hurtEffectCount -= 1;

	std::vector<NormalBullet*>* listNormalBullet = &NormalBullet::listNormalBullet;
	for (std::vector<NormalBullet*>::iterator it = listNormalBullet->begin(); it != listNormalBullet->end(); it++)
	{
			(*it)->Draw();
	}
}

void Megaman::OnCollision(GameObject * object, float collideTime, int normalX, int normalY)
{
}

void Megaman::OnCollision(float deltatime)
{
	///////////////////////////////Check With Enemy//////////////////////////////////////////////
	///////////////Check With NotorBanger////////////////////////////
	std::vector<NotorBanger*> *listNotorBanger = &NotorBanger::listNotorBanger;
	if (!listNotorBanger->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<NotorBanger*>::iterator it = listNotorBanger->begin(); it != listNotorBanger->end(); it++)
		{
			if ((*it)->IsDisable() || state == eMegamanState::Hurt || bImmortal)
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			state = eMegamanState::Hurt;

			if (this->box.GetPosition().x - (*it)->box.GetPosition().x > 0)
				EnemyCollision(1);		//Right
			else
				EnemyCollision(-1);		//Left

		}
	}
	///////////////Check With HeadGunner////////////////////////////
	std::vector<HeadHunter*> *listHeadHunter = &HeadHunter::listHeadHunter;
	if (!listHeadHunter->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<HeadHunter*>::iterator it = listHeadHunter->begin(); it != listHeadHunter->end(); it++)
		{
			if ((*it)->IsDisable() || state == eMegamanState::Hurt || bImmortal)
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			state = eMegamanState::Hurt;

			if (this->box.GetPosition().x - (*it)->box.GetPosition().x > 0)
				EnemyCollision(1);		//Right
			else
				EnemyCollision(-1);		//Left
		}
	}
	///////////////Check With Helit////////////////////////////
	std::vector<Helit*> *listHelit = &Helit::listHelit;
	if (!listHelit->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<Helit*>::iterator it = listHelit->begin(); it != listHelit->end(); it++)
		{
			if ((*it)->IsDisable() || state == eMegamanState::Hurt || bImmortal)
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			state = eMegamanState::Hurt;

			if (this->box.GetPosition().x - (*it)->box.GetPosition().x > 0)
				EnemyCollision(1);		//Right
			else
				EnemyCollision(-1);		//Left
		}
	}
	///////////////Check With NotorBullet////////////////////////////
	std::vector<NotorBullet*> *listNotorBullet = &NotorBullet::listNotorBullet;
	if (!listNotorBullet->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<NotorBullet*>::iterator it = listNotorBullet->begin(); it != listNotorBullet->end(); it++)
		{
			if ((*it)->bDisable || state == eMegamanState::Hurt || bImmortal)
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			state = eMegamanState::Hurt;

			if (this->box.GetPosition().x - (*it)->box.GetPosition().x > 0)
				EnemyCollision(1);		//Right
			else
				EnemyCollision(-1);		//Left
		}
	}
	///////////////Check With NotorBullet////////////////////////////
	std::vector<Rocket*> *listRocket = &Rocket::listRocket;
	if (!listRocket->empty() && !DEBUG_IMMORTAL)
	{
		for (std::vector<Rocket*>::iterator it = listRocket->begin(); it != listRocket->end(); it++)
		{
			if ((*it)->bDisable || state == eMegamanState::Hurt || bImmortal)
				continue;

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			state = eMegamanState::Hurt;

			if (this->box.GetPosition().x - (*it)->box.GetPosition().x > 0)
				EnemyCollision(1);		//Right
			else
				EnemyCollision(-1);		//Left
		}
	}

	///////////////////////////////Check With MapCollision//////////////////////////////////////////////
	std::vector<MapCollision*> *listMapCollsion = &MapCollision::listMapCollision;
	if (!listMapCollsion->empty())
	{
		for (std::vector<MapCollision*>::iterator it = listMapCollsion->begin(); it != listMapCollsion->end(); it++)
		{
			if ((*it)->IsDisable())
				continue;


			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), (*it)->box.GetBox()))
				continue;
			int normalX = 0;
			int normalY = 0;
			float collideTime = Collision::GetCollideTime(this->box, (*it)->box, &normalX, &normalY, deltatime);
			
			if ((*it)->name == "ground")
			{
				GroundCollision(collideTime, normalX, normalY, deltatime, (*it)->box.GetBox());
			}

			if ((*it)->name == "leftScroller")
			{
				GroundCollision(collideTime, normalX, normalY, deltatime, (*it)->box.GetBox());
				GetMoveComponent()->SetVelocity(GetMoveComponent()->GetVelocity().x - 70, GetMoveComponent()->GetVelocity().y);
			}

			if ((*it)->name == "rightScroller")
			{
				GroundCollision(collideTime, normalX, normalY, deltatime, (*it)->box.GetBox());
				GetMoveComponent()->SetVelocity(GetMoveComponent()->GetVelocity().x + 70, GetMoveComponent()->GetVelocity().y);
			}

			if ((*it)->name == "wall" && normalX != 0)
			{
				WallCollision(collideTime, normalX, normalY, deltatime, (*it)->box.GetBox());
				
			}
		}
	}

	///////////////////////////////Check With Elevator//////////////////////////////////////////////
	std::vector<Elevator*> *listElevator = &Elevator::listElevator;
	if (!listElevator->empty())
	{
		for (std::vector<Elevator*>::iterator it = listElevator->begin(); it != listElevator->end(); it++)
		{
			int normalX = 0;
			int normalY = 0;
			float collideTime3 = Collision::GetCollideTime(this->box, (*it)->box, &normalX, &normalY, deltatime);

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;
			
			float collideTime = Collision::GetCollideTime(this->box, (*it)->box, &normalX, &normalY, deltatime);

			if ((*it)->name == "ground")
				ElevatorCollision(collideTime, normalX, normalY, deltatime, (*it)->box.GetBox(),(*it)->GetMoveComponent()->GetSpeed());
			
		}
	}

	///////////////////////////////Check With Gate//////////////////////////////////////////////
	std::vector<Gate1*> *listGate1 = &Gate1::listGate1;
	if (!listGate1->empty())
	{
		for (std::vector<Gate1*>::iterator it = listGate1->begin(); it != listGate1->end(); it++)
		{
			int normalX = 0;
			int normalY = 0;
			float collideTime3 = Collision::GetCollideTime(this->box, (*it)->box, &normalX, &normalY, deltatime);

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Collision::GetBroadphaseBox((*it)->box, deltatime)))
				continue;

			float collideTime = Collision::GetCollideTime(this->box, (*it)->box, &normalX, &normalY, deltatime);

			if (this->GetPosition().x - (*it)->GetPosition().x < 0)
			{
				if ((*it)->name == "gate" && !bPassingGate)
				{
					bPassingGate = true;
					sprite.get()->ToggleAnimationClip();
					bDisableInput = true;
					GetMoveComponent()->IdleX();
					GetMoveComponent()->SetVelocity(0, GetMoveComponent()->GetVelocity().y);
					(*it)->GateOpen();
					tempGate = (*it);
				}
			}
			else
				GateCollision(collideTime, normalX, normalY, deltatime, (*it)->box.GetBox(), (*it)->GetMoveComponent()->GetSpeed());
		}
	}

	
}

void Megaman::Destroy()
{
}

void Megaman::OnKeyDown(int Keycode)
{
	switch (Keycode)
	{
	case DIK_K:
		Jump();
		break;
	default:
		break;
	}
}

void Megaman::OnKeyUp(int Keycode)
{
	switch (Keycode)
	{
	case DIK_L:
		bDash = false;
		dashTime = limitDashTime;
		GetMoveComponent()->OffDashing();
		break;
	case DIK_J:
		FireBullet();
	default:
		break;
	}
}

Megaman* Megaman::instance;

Megaman* Megaman::getInstance()
{
	if (instance == nullptr)
		instance = new Megaman();
	return instance;
}

