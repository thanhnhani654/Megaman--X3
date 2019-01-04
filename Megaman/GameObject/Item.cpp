#include "Item.h"

std::vector<Item*> Item::listItem;

Item::Item(D3DXVECTOR2 pos, int direct)
{
	Initialize();
	SetPosition(pos);
	//demTime = 0;
}

bool Item::IsDisable()
{
	return bDisable;
}

void Item::Disable()
{
	box.Disable();
	bDisable = true;
}

void Item::Enable()
{
	bDisable = false;
	box.Enable();
}

void Item::Initialize()
{
	Creature::Initialize();
	GetMoveComponent()->EnableGravity();
	GetMoveComponent()->SetSpeed(150);

	box.DynamicInitialize(this, 16, 12);
	box.SetPivot(8, 6);

	sprite.get()->SetAnimation("energy_capsule");
	lifeTime = 10;
	//megaman = Megaman::getInstance();
	listItem.push_back(this);

}

void Item::ReInitialize(D3DXVECTOR2 pos, int direct)
{
	lifeTime = 10;
	this->SetPosition(pos);

	this->Enable();
}


void Item::Update(float deltatime)
{
	OnCollision(deltatime);

	GetMoveComponent()->UpdateMovement(deltatime);

	box.SetPosition();
	lifeTime -= deltatime;
	if (lifeTime < 0)
		Disable();
}

void Item::WallCollision(float collideTime, int normalX, int normalY, float deltatime, Box box)
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

			if (collideTime != 0)
			{
				//cout << "CollideTime: " << collideTime << "\tVeclocity: " << vecY << "\tPosition: " << GetPosition().y - 16 << endl;
				//cout << "Va cham ben duoi" << endl;
			}
		}

		GetMoveComponent()->SetVelocity(vecX, vecY);
	}
}

void Item::OnCollision(float deltatime)
{
	// Kiểm tra va chạm với Ground
	std::vector<MapCollision*> *listMapCollsion = &MapCollision::listMapCollision;
	if (!listMapCollsion->empty())
	{
		for (std::vector<MapCollision*>::iterator it = listMapCollsion->begin(); it != listMapCollsion->end(); it++)
		{

			if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), (*it)->box.GetBox()))
				continue;
			//int normalX = 0;
			//int normalY = 0;
			//float collideTime = Collision::GetCollideTime(this->box, (*it)->box, &normalX, &normalY, deltatime);

			//WallCollision(collideTime, normalX, normalY, deltatime, (*it)->box.GetBox());
			this->GetMoveComponent()->DisableGravity();
			this->GetMoveComponent()->IdleY();
		}
	}

	if (!Collision::IsIntersection(Collision::GetBroadphaseBox(this->box, deltatime), Megaman::getInstance()->box.GetBox()))
		return;
	Disable();
	Megaman::getInstance()->GetHPComponent()->Health(3);
}

void Item::Draw()
{
	Creature::Draw();
}

void Item::Destroy()
{
}

void Item::UpdateAll(float deltatime)
{
	if (PRINT_SIZE && !PRINT_SIZE_ENABLE_OBJECT)
	{
		static float printTimeCount = 0;
		if (printTimeCount >= 1.0)
		{
			cout << "Item's Object Amount: " << listItem.size() << endl;
			printTimeCount = 0;
		}
		printTimeCount += deltatime;
	}

	for (std::vector<Item*>::iterator it = listItem.begin(); it != listItem.end(); it++)
	{
		if (!(*it)->IsDisable())
			(*it)->Update(deltatime);
	}

}

void Item::DrawAll()
{
	for (std::vector<Item*>::iterator it = listItem.begin(); it != listItem.end(); it++)
	{
		if (!(*it)->IsDisable())
			(*it)->Draw();
	}
}

Item* Item::CreateItem(D3DXVECTOR2 pos, int direct)
{
	for (int i = 0; i < listItem.size(); i++)
	{
		if (listItem[i]->IsDisable())
		{
			listItem[i]->ReInitialize(pos, direct);
			return listItem[i];
		}
	}
	Item* notor = new Item(pos, direct);
	return notor;
}
