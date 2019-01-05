#pragma once
#include "GameObject\GameObject.h"
#include "GameObject\Megaman.h"
#include "GameObject\Camera.h"
#include <fstream>
#include <vector>
#include "Sprite.h"
#include "GameObject\Map.h"
#include "GameObject\MapCollision.h"
#include "GameObject\NotorBanger.h"
#include "GameObject\Elevator.h"
#include "GameObject\HeadHunter.h"
#include "GameObject\Helit.h"
#include "GameObject\NotorBullet.h"
#include "GameObject\Rocket.h"
#include "GameObject\HelitRocket.h"
#include "GameObject\Gate1.h"
#include "GameObject\BlastHornet.h"
#include "GameObject\Bee.h"
#include "GameObject\HUD.h"
#include "GameObject\Item.h"
#include "GameObject\Layer2.h"

#include "Quadtree.h"
using namespace std;



class GameScene
{
private:
	fstream f;

	Megaman* megaman;
	Map map;
	MapCollision mapcollision1;
	MapCollision mapcollision2;
	MapCollision mapcollision3;
	MapCollision mapcollision4;

	Elevator ele;
	Gate1 gate1;

	Quadtree* tree;

public:

	void Initialize();
	void UpdateInput(float deltatime);
	void Update(float deltatime);
	void Draw();

	template <class T>
	Creature* Spawner(D3DXVECTOR2 pos) 
	{
		return new T(pos);
	}

	template <class T>
	Creature* Spawner(D3DXVECTOR2 pos, bool direction)
	{
		return new T(pos, direction);
	}

	
};