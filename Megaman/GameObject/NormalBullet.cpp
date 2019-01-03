#include "NormalBullet.h"

std::vector<NormalBullet*> NormalBullet::listNormalBullet;

void NormalBullet::Ghost_Initialize(float x, float y, eDirection idirection, int lv)
{
	GameObject::Ghost_Initialize();
	lifeTime = 5.0f;
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	speed = 5;
	box.DynamicInitialize(this, 8, 6);
	switch (lv)
	{
	case 1:
		sprite.get()->SetAnimation("char_bullet");
		break;
	case 2:
		sprite.get()->SetAnimation("char_bullet_lv2");
		break;
	case 3:
		sprite.get()->SetAnimation("char_bullet_lv3");
		break;
	}

	if (idirection == eDirection::Left)
		sprite.get()->FlipLeft();
	
	GetTagMethod()->AddTag(eTag::PlayerBulletTag);

	listNormalBullet.push_back(this);
}

void NormalBullet::Re_Initialize(float x, float y, eDirection idirection, int lv)
{
	lifeTime = 5.0f;
	bDisable = false;
	SetPosition(x, y);
	this->direction = idirection;
	speed = 5;

	switch (lv)
	{
	case 1:
		sprite.get()->SetAnimation("char_bullet");
		break;
	case 2:
		sprite.get()->SetAnimation("char_bullet_lv2");
		break;
	case 3:
		sprite.get()->SetAnimation("char_bullet_lv3");
		break;
	}

	if (idirection == eDirection::Left)
		sprite.get()->FlipLeft();
}

void NormalBullet::CountDown(float deltatime)
{
	lifeTime -= deltatime;
}

void NormalBullet::Disable()
{
	bDisable = true;
}

void NormalBullet::Update(float deltatime)
{
	if (bDisable)
		return;
	if (direction == eDirection::Left)
		GetPosition_Ptr()->x -= speed;
	else
		GetPosition_Ptr()->x += speed;
	box.SetPosition();

	CountDown(deltatime);
	if (lifeTime <= 0)
		Disable();
}

void NormalBullet::Draw()
{
	if (!bDisable)
	GameObject::Draw();
}
