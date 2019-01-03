#pragma once
#include "GameObject.h"
#include "../Component/Box2D.h"

enum eItem
{
	HealthPackItem,
	RocketItem,
	FreezeItem,
};

class Item : public GameObject
{
private:
	eItem _Item;
	Box2D box;
public:

	eItem GetItemType();

	void Initialize(eItem item);
	void Destroy();
	void Draw();
};