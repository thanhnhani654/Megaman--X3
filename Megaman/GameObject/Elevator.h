#pragma once
#include "../Component/Box2D.h"
#include "Creature.h"
#include <vector>


class Elevator : public Creature
{
private:


public:
	static std::vector<Elevator*> listElevator;
	string name;
	int id;
	Box2D box;

	void Initialize();
	void Initialize(float x, float y, int w, int h);

	void SetName(string name)
	{
		this->name = name;
	}

	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void Draw();
};