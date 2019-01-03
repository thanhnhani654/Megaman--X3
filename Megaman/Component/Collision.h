#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "Box2D.h"
#include "../GameObject/GameObject.h"
#include <math.h>
#include <vector>

struct CheckBox
{
	Box2D box;
	float dist;
};

class Collision
{
public:
	static Collision* instance;
	static Collision* getInstance();

	std::vector<CheckBox> _CheckListBox;


	static float GetCollideTime(Box2D object1, Box2D object2, int* normalx, int* normaly, float deltatime);

	static bool IsIntersection(Box object1, Box object2);
	static Box GetBroadphaseBox(Box2D &object, float delta);
	float Distance(Box object1, Box object2);
	void AddCheckListBox(Box2D &object, float dist);
	void doCollision(Box2D object, float deltatime);

	void CollisionChecker(float deltatime);

	void BoxUpdater();
};