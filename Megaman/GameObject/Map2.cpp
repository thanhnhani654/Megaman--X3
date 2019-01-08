#include "Map2.h"

void Map2::Initialize()
{
	GameObject::Initialize();
	sprite.get()->SetAnimation("map2");
}

void Map2::Draw()
{
	GameObject::Draw();
}