#include "Map.h"

void Map::Initialize()
{
	GameObject::Initialize();
	sprite.get()->SetAnimation("map");
}

void Map::Draw()
{
	GameObject::Draw();
}