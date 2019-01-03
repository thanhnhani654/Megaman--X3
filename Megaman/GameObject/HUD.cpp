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
	HPBar.SetAnimation("explosive_1");
	HpUnit.SetAnimation("explosive_2");
	_camera = Camera::getInstance();
}

void HUD::Update(float deltatime)
{
}

void HUD::Draw()
{
	HPBar.Render(120 + _camera->GetPosition().x, 200 + _camera->GetPosition().y);
	HpUnit.Render(80 + _camera->GetPosition().x, 200 + _camera->GetPosition().y);
}
