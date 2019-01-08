#pragma once
//#include "Creature.h"
#include "Megaman.h"
#include "Camera.h"

struct HpUnits
{
	Sprite unit;
	D3DXVECTOR2 pos;
};

class HUD : public GameObject
{
private:
	static HUD* instance;

	Sprite CharHPBar;
	Sprite BossHPBar;
	Sprite HpUnit;
	int CharHp;
	int BossHp;
	

	Camera* _camera;
public:
	static HUD* getInstance();

	std::vector<HpUnits> CharHpUnits;
	std::vector<HpUnits> BossHpUnits;

	bool bBossDisable;

	void Ghost_Initialize();		
	void CreateCharHpUnit();
	void CreateBossHpUnit();
	void Update(float deltatime);
	void UpdateMegamanHP();
	void GetBossHp(int hp);
	void Draw();
};