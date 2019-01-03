#pragma once
#include "../Component/Box2D.h"
#include "Creature.h"
#include <vector>


class Gate1 : public Creature
{
private:


public:
	static std::vector<Gate1*> listGate1;
	string name;
	int id;
	Box2D box;

	void Initialize();
	void Initialize(float x, float y, int w, int h);

	void SetName(string name)
	{
		this->name = name;
	}

	void Update(float deltatime);
	void Draw();
};