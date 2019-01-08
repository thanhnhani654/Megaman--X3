#include "HPComponent.h"

void HPComponent::Initialize(float hp, float dmg)
{
	healtPoint = hp;
	damage = dmg;
	bImmortal = false;
	bGodMode = false;
	bDead = false;
}

void HPComponent::DoDamage(float dmg, bool godmode)
{
	if (godmode)
		healtPoint = -1;
	if (!bImmortal)
		healtPoint -= dmg;

	if (healtPoint < 0)
	{
		Dead();
		//std::cout << "Dead" << std::endl;
	}

}

void HPComponent::ToggleImmortal()
{
	bImmortal = !bImmortal;
}

void HPComponent::ToggleGodMode()
{
	bGodMode = !bGodMode;
}
