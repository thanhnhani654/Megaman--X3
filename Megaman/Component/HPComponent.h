#pragma once
#include <iostream>

class HPComponent
{
private:
	float maxHealtPoint;
	float healtPoint;
	float damage;
	bool bDead;
	bool bImmortal;
	bool bGodMode;
public:

	void Initialize(float hp, float dmg);

	void	SetMaxHP(float health) { maxHealtPoint = health; }
	float	GetMaxHP() { return maxHealtPoint; }
	void	SetHP(float health) { healtPoint = health; }
	float	GetHP() { return healtPoint; }
	void	SetDamage(float dmg) { damage = dmg; }
	float	GetDamage() { return damage; }
	void	Dead() { bDead = true; }
	bool	IsDead(){ return bDead; }
	bool	IsGodMode() { return bGodMode; }
	void DoDamage(float dmg, bool godmode = false);
	void Health(float heal);
	void ToggleImmortal();
	void ToggleGodMode();

};