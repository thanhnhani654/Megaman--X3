#include "HUD.h"

HUD* HUD::instance;

HUD* HUD::getInstance()
{
	if (instance == nullptr)
		instance = new HUD();
	return instance;
}

void HUD::Ghost_Initialize()
{
	CharHPBar.SetAnimation("char_hp_bar");
	BossHPBar.SetAnimation("boss_hp_bar");
	HpUnit.SetAnimation("hp_unit");
	_camera = Camera::getInstance();
}

void HUD::Update(float deltatime)
{
}

void HUD::Draw()
{
	CharHPBar.Render(20 + _camera->GetPosition().x, 280 + _camera->GetPosition().y);
	HpUnit.Render(20 + _camera->GetPosition().x, 261 + _camera->GetPosition().y);


	BossHPBar.Render(50 + _camera->GetPosition().x, 280 + _camera->GetPosition().y);
}
