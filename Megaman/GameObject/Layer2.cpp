#include "Layer2.h"

std::vector<Layer2*> Layer2::listLayer2;

Layer2::Layer2(D3DXVECTOR2 pos, int sprite)
{
	Initialize(sprite);
	SetPosition(pos);
}

void Layer2::Initialize(int sprite)
{
	GameObject::Initialize();
	switch (sprite)
	{
	case 0:
		this->sprite.get()->SetAnimation("gate_1");
		break;
	case 1:
		break;
	}

	listLayer2.push_back(this);
}

void Layer2::Draw()
{
	GameObject::Draw();
}

void Layer2::DrawAll()
{
	for (std::vector<Layer2*>::iterator it = listLayer2.begin(); it != listLayer2.end(); it++)
	{
			(*it)->Draw();
	}
}

Layer2 * Layer2::CreateLayer2(D3DXVECTOR2 pos, int sprite)
{
	Layer2* notor = new Layer2(pos, sprite);
	return notor;
}

Layer2 * Layer2::CreateLayer2(float x, float y, int sprite)
{
	D3DXVECTOR2 pos;
	pos.x = x;
	pos.y = y;
	Layer2* notor = new Layer2(pos, sprite);
	return notor;
}
