#pragma once
#include "../Component/Box2D.h"
#include "GameObject.h"
#include <vector>

// May cai Ground voi Wall
class MapCollision : public GameObject
{
private:
	
	
public:
	static std::vector<MapCollision*> listMapCollision;
	string name;
	int id;
	Box2D box;

	void Initialize();
	void Initialize(int id,float x, float y, int w, int h);
	void Initialize2(int type, float x, float y, int w, int h);
	void ReInitialize(int type, float x, float y, int w, int h);
	static MapCollision* CreateMapCollision(int type, float x, float y, int w, int h);
	void Disable();
	void Enable();
	bool IsDisable();

	void SetName(string name)
	{
		this->name = name;
	}

	static void LoadMapCollision();

	static void PrintList();
};