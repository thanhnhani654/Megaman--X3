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
	_camera = Camera::getInstance();
	CharHPBar.SetAnimation("char_hp_bar");
	BossHPBar.SetAnimation("boss_hp_bar");
	//HpUnit.SetAnimation("hp_unit");
	CreateCharHpUnit();
	CreateBossHpUnit();
	bBossDisable = true;
}

void HUD::CreateCharHpUnit()
{
	int y = 150;
	
	for (int i = 0; i < 19; i++)
	{
		HpUnits unit;
		unit.unit.SetAnimation("hp_unit");
		unit.pos.x = 30 ;
		unit.pos.y = y;
		CharHpUnits.push_back(unit);
		y += 3;
	}
}

void HUD::CreateBossHpUnit()
{
	int y = 140;
	for (int i = 0; i < 25; i++)
	{
		HpUnits unit;
		unit.unit.SetAnimation("hp_unit");
		unit.pos.x = 260;
		unit.pos.y = y;
		BossHpUnits.push_back(unit);
		y += 3;
	}
}

void HUD::Update(float deltatime)
{
	UpdateMegamanHP();
}

void HUD::UpdateMegamanHP()
{
	CharHp = Megaman::getInstance()->GetHPComponent()->GetHP();
	if (CharHp < 0)
		CharHp = 0;
}

void HUD::GetBossHp(int hp)
{
	BossHp = hp;

	if (hp < 0)
		BossHp = 0;
}
	

void HUD::Draw()
{
	CharHPBar.Render(30 + _camera->GetPosition().x, 170 + _camera->GetPosition().y);
	//HpUnit.Render(30 + _camera->GetPosition().x, 150 + _camera->GetPosition().y);
	for (int i = 0; i < CharHp; i++)
	{
		CharHpUnits[i].unit.Render(CharHpUnits[i].pos.x + _camera->GetPosition().x, CharHpUnits[i].pos.y + _camera->GetPosition().y);
	}

	BossHPBar.Render(260 + _camera->GetPosition().x, 170 + _camera->GetPosition().y);
	for (int i = 0; i < BossHp; i++)
	{
		BossHpUnits[i].unit.Render(BossHpUnits[i].pos.x + _camera->GetPosition().x, BossHpUnits[i].pos.y + _camera->GetPosition().y);
	}
}
