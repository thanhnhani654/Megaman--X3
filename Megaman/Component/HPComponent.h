#pragma once
#include <iostream>

class HPComponent
{
private:
	float healtPoint;
	float damage;
	bool bDead;
	bool bImmortal;
	bool bGodMode;
public:

	void Initialize(float hp, float dmg);

	void	SetHP(float health) { healtPoint = health; }
	float	GetHP() { return healtPoint; }
	void	SetDamage(float dmg) { damage = dmg; }
	float	GetDamage() { return damage; }
	void	Dead() { bDead = true; }
	bool	IsDead(){ return bDead; }
	bool	IsGodMode() { return bGodMode; }
	void DoDamage(float dmg, bool godmode = false);
	void ToggleImmortal();
	void ToggleGodMode();

};