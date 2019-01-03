#include "Item.h"

void Item::Initialize(eItem item)
{
	GameObject::Initialize();
	_Item = item;
	GetTagMethod()->AddTag(eTag::ItemTag);

	switch (_Item)
	{
	case HealthPackItem:
		sprite.get()->SetAnimation("Tile_23");
		break;
	case RocketItem:
		sprite.get()->SetAnimation("Tile_23");
		break;
	case FreezeItem:
		sprite.get()->SetAnimation("Tile_23");
		break;
	default:
		break;
	}

	box.StaticInitialize(this, 16, 16);
}

eItem Item::GetItemType()
{
	return _Item;
}

void Item::Destroy()
{
	GameObject::Destroy();
	box.SetDeleted();
}

void Item::Draw()
{
	if (sprite.get() != nullptr)
		GameObject::Draw();
}