#include "GameObject.h"

// Khai báo các biến toàn cục
#pragma region Define
std::vector<std::reference_wrapper<GameObject>> GameObject::listObject;
#pragma endregion

void GameObject::Initialize()
{
	sprite = std::make_unique<Sprite>();
	listObject.push_back(*this);
}

void GameObject::Ghost_Initialize()
{
	sprite = std::make_unique<Sprite>();
}

void GameObject::UpdateInput(float deltatime)
{

}

void GameObject::Update(float deltatime)
{

}

void GameObject::Draw()
{
	if (sprite != nullptr)
		sprite.get()->Render(position.x, position.y);
}

void GameObject::Destroy()
{
	if (childObject.size() != 0)
	{
		for (int i = 0; i < childObject.size(); i++)
		{
			childObject[i].get().SetDeleted();
		}
		childObject.clear();
	}

	if (sprite != nullptr)
		sprite.reset();

}

GameObject* GameObject::FindObjectWithTag(eTag tag)
{
	for (int i = 0; i < listObject.size(); i++)
	{
		if (listObject[i].get().tags.isHasTag(tag))
		{
			return &listObject[i].get();
		}
	}
	return nullptr;
}

void GameObject::DestroyProcess()
{
	int mark = -1;
	std::vector<std::reference_wrapper<GameObject>>::iterator it;
	for (it = listObject.begin(); it != listObject.end(); it++)
	{
		if ((*it).get().deleted)
		{
			(*it).get().Destroy();
			mark = 1;
			break;
		}
	}
	if (mark != (-1))
	{
		listObject.erase(it);
		listObject.shrink_to_fit();
		DestroyProcess();
	}
}