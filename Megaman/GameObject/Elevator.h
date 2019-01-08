#pragma once
#include "../Component/Box2D.h"
#include "Creature.h"
#include <vector>


class Elevator : public Creature
{
private:

	float stopPoint;
public:
	static std::vector<Elevator*> listElevator;
	string name;
	int id;
	Box2D box;
	bool bWaiting;
	bool bStop;

	void Initialize();
	void Initialize(float x, float y, int w, int h);

	void SetName(string name)
	{
		this->name = name;
	}

	void Reset();

	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void Draw();
};